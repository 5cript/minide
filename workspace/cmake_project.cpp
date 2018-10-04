#include "cmake_project.hpp"
#include "project_impl.hpp"
#include "globber.hpp"
#include "project_file/local.hpp"
#include "debugger/gdb.hpp"

#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <stdexcept>
#include <cstdio>

using namespace std::string_literals;

namespace MinIDE
{
//#####################################################################################################################
    struct CMakeProjectImpl
    {
        GlobalPersistence* settings;

        std::unique_ptr <AsyncProcess> process;
        GdbCommunicator gdb;

        CMakeProjectImpl(CMakeProject& owner, GlobalPersistence* settings)
            : settings{settings}
            , process{}
            , gdb{&owner.getEventManager()}
        {
        }
    };
//#####################################################################################################################
    void DebugCommands::step()
    {
        //project->cmakeImpl_->gdb.step();
    }
//---------------------------------------------------------------------------------------------------------------------
    void DebugCommands::nextLine()
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void DebugCommands::stepInto()
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void DebugCommands::stepOut()
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void DebugCommands::nextInstruction()
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void DebugCommands::stepIntoInstruction()
    {

    }
//#####################################################################################################################
    CMakeProject::CMakeProject(GlobalPersistence* settings, path const& rootDir)
        : Project{settings}
        , cmakeImpl_{new CMakeProjectImpl(*this, settings)}
    {
        load(rootDir);
    }
//---------------------------------------------------------------------------------------------------------------------
    CMakeProject::~CMakeProject() = default;
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::load(path const& rootDir)
    {
        Project::load(rootDir);

        // Should never be possible to throw. Safeguard.
        if (!filesystem::exists(rootDir / "CMakeLists.txt"))
            throw std::invalid_argument("This is not a CMake directory");

        glob(
            rootDir,
            impl_->settings->cmakeBaseSettings.globbing.masks,
            impl_->settings->cmakeBaseSettings.globbing.dirBlacklist
        );

        //_putenv(("PATH="s + environment_->path).c_str());
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string CMakeProject::type() const
    {
        return "cmake";
    }
//---------------------------------------------------------------------------------------------------------------------
    ProjectPersistence::CMakeBuildProfile* CMakeProject::getTarget(std::string const& target)
    {
        return static_cast <ProjectPersistence::CMakeBuildProfile*> (Project::getTarget(target));
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::addTarget(ProjectPersistence::CMakeBuildProfile const& target)
    {
        impl_->localFile.buildProfiles.push_back(
            std::make_unique <ProjectPersistence::CMakeBuildProfile> (target)
        );
        saveProjectFiles();
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::buildStep(int step, std::string const& target)
    {
        auto* targ = getTarget(target);
        if (targ == nullptr)
            throw std::invalid_argument("Such a target does not exist.");

        switch (step)
        {
        case 0:
            runCMake(targ);
            break;
        case 1:
            runMake(targ);
            break;
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::runExternal(
        std::string const& command,
        std::string const& outputPipe,
        ProjectPersistence::CMakeBuildProfile* target
    )
    {
        auto* settings = getSettings();

        // find environment
        auto environment_iter = settings->environments.find(target->environment);
        if (environment_iter == std::end(settings->environments))
            throw std::runtime_error("the local project asked for an environment setting that does not exist on this computer");

        // set environment
        std::unordered_map <std::string, std::string> env = environment_iter->second.compile();

        // set other variables
        auto buildDirectory = buildDir(target);
        if (target->outputIsRelative)
            buildDirectory = rootDir() / buildDir(target);

        cmakeImpl_->process = std::make_unique <AsyncProcess> (
            command,
            buildDirectory.string(),
            env,
            [this, outputPipe](std::string const& param) {
                events_.callNamed(ProjectEvents::ProcessOutput, outputPipe, param);
            },
            [this](int param) {
                events_.callNamed(ProjectEvents::ProcessExited, "process_exit", param);
                //cmakeImpl_->process.reset(nullptr);
            }
        );
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::killProcess()
    {
        if (cmakeImpl_->process && cmakeImpl_->process->isRunning())
            cmakeImpl_->process->kill();
        else if (cmakeImpl_->gdb.isRunning())
        {
            if (cmakeImpl_->gdb.wasStopped())
                cmakeImpl_->gdb.kill();
            else
                cmakeImpl_->gdb.stop();
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::runCMake(ProjectPersistence::CMakeBuildProfile* target)
    {
        auto* settings = getSettings();

        // find toolbox
        auto tooling_iter = settings->tooling.find(target->toolProfile);
        if (tooling_iter == std::end(settings->tooling))
            throw std::runtime_error("the local project asked for a tooling profile that does not exist on this computer");
        auto cmake = tooling_iter->second.cmake;

        std::string options;
        if (target->cmakeOptions)
            options = target->cmakeOptions.value();

        runExternal(
            cmake + " \"" + rootDir().string() + "\" " + options,
            "build_log",
            target
        );
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::runMake(ProjectPersistence::CMakeBuildProfile* target)
    {
        auto* settings = getSettings();

        // find toolbox
        auto tooling_iter = settings->tooling.find(target->toolProfile);
        if (tooling_iter == std::end(settings->tooling))
            throw std::runtime_error("the local project asked for a tooling profile that does not exist on this computer");
        auto make = tooling_iter->second.make;

        std::string options;
        if (target->makeOptions)
            options = target->makeOptions.value();

        runExternal(make + " " + options, "build_log", target);
    }
//---------------------------------------------------------------------------------------------------------------------
    path CMakeProject::buildDir(ProjectPersistence::CMakeBuildProfile* target) const
    {
        return target->outputPath;
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::run(std::string const& target)
    {
        auto* targ = getTarget(target);

        auto buildDirectory = buildDir(targ);
        if (targ->outputIsRelative)
            buildDirectory = rootDir() / buildDir(targ);

        if (targ->executable)
            runExternal((buildDirectory / targ->executable.value()).string(), "process_output", targ);
        else
            events_.callNamed(ProjectEvents::Info, "info", "target lacks executable name parameter");
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::runDebug(std::string const& target, std::string const& debuggerProfile)
    {
        auto* targ = getTarget(target);

        if (!targ->executable)
        {
            events_.callNamed(ProjectEvents::Info, "info", "target lacks executable name parameter");
            return;
        }

        auto buildDirectory = buildDir(targ);
        if (targ->outputIsRelative)
            buildDirectory = rootDir() / buildDir(targ);

        auto debuggerSettings = impl_->settings->debugger.at(debuggerProfile);
        if (targ->debuggerSettings)
        {
            debuggerSettings = targ->debuggerSettings.value();
            events_.callNamed(ProjectEvents::Info, "info", "using project locally defined debugger settings");
        }

        // find environment
        auto environment_iter = impl_->settings->environments.find(targ->environment);
        if (environment_iter == std::end(impl_->settings->environments))
            throw std::runtime_error("the local project asked for an environment setting that does not exist on this computer");

        cmakeImpl_->gdb.run(rootDir(), debuggerSettings, targ, environment_iter->second);
    }
//---------------------------------------------------------------------------------------------------------------------
    bool CMakeProject::isDebugging() const
    {
        return cmakeImpl_->gdb.isRunning();
    }
//---------------------------------------------------------------------------------------------------------------------
    DebugCommands CMakeProject::debugCommands()
    {
        return DebugCommands{this};
    }
//#####################################################################################################################
}
