#include "cmake_project.hpp"
#include "project_impl.hpp"
#include "globber.hpp"

#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <stdexcept>
#include <cstdio>

using namespace std::string_literals;

namespace MinIDE
{
//#####################################################################################################################
    CMakeProject::CMakeProject(GlobalPersistence* settings, path const& rootDir)
        : Project{settings}
    {
        load(rootDir);
    }
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
    void CMakeProject::runExternal(std::string const& command, ProjectPersistence::CMakeBuildProfile* target)
    {
        auto* settings = getSettings();

        // find environment
        auto environment_iter = settings->environments.find(target->environment);
        if (environment_iter == std::end(settings->environments))
            throw std::runtime_error("the local project asked for an environment setting that does not exist on this computer");

        // set environment
        std::unordered_map <std::string, std::string> env;
        env["PATH"] = environment_iter->second.path;
        env.insert(std::begin(environment_iter->second.variables), std::end(environment_iter->second.variables));

        // set other variables
        auto buildDirectory = buildDir(target);
        if (target->outputIsRelative)
            buildDirectory = rootDir() / buildDir(target);

        process_ = std::make_unique <AsyncProcess> (
            command,
            buildDirectory.string(),
            env,
            callback()
        );
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
            options = target->cmakeOptions.get();

        runExternal(
            cmake + " \"" + rootDir().string() + "\" " + options,
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
            options = target->makeOptions.get();

        runExternal(make + " " + options, target);
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
            runExternal((buildDirectory / targ->executable.get()).string(), targ);
        else
            callback()("target lacks executable name parameter");
    }
//---------------------------------------------------------------------------------------------------------------------
    void loadEnvironment(ProjectPersistence::CMakeBuildProfile* target)
    {

    }
//#####################################################################################################################
}
