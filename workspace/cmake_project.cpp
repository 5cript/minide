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
        static_cast <ProjectPersistence::CMakeBuildProfile*> (Project::getTarget(target));
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
    void CMakeProject::buildStep(int step, ProjectPersistence::CMakeBuildProfile* target)
    {
        switch (step)
        {
        case 0:
            runCMake(target);
            break;
        case 1:
            runMake(target);
            break;
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::runCMake(ProjectPersistence::CMakeBuildProfile* target)
    {
        /*
        auto dir = buildDir(debug);

        process_ = std::make_unique <AsyncProcess> (
            settings_->toolDirectory
                + "/"
                + settings_->cmakeCommand
                + " \""s
                + rootDir_.string()
                + "\" "
                + settings_->cmakeProjectSettings.cmakeParameters
            , dir.string()
            , cb_
        );
        */
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::runMake(ProjectPersistence::CMakeBuildProfile* target)
    {
        /*
        auto dir = buildDir(debug);

        process_ = std::make_unique <AsyncProcess> (
            settings_->toolDirectory
                + "/"
                + settings_->makeCommand
                + " -C \""s
                + dir.string()
                + "\""
            , dir.string()
            , cb_
        );
        */
    }
//---------------------------------------------------------------------------------------------------------------------
    path CMakeProject::buildDir(ProjectPersistence::CMakeBuildProfile* target) const
    {
        /*
        auto dir = rootDir_;
        if (debug)
            dir /= settings_->cmakeProjectSettings.debugOutputDir;
        else
            dir /= settings_->cmakeProjectSettings.releaseOutputDir;
        filesystem::create_directory(dir);
        return dir;
        */
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::run(std::string const& target)
    {
        /*
        auto dir = buildDir(debug);
        process_ = std::make_unique <AsyncProcess> (
            (dir / "faky.exe").string(),
            dir.string(),
            cb_
        );
        */
    }
//---------------------------------------------------------------------------------------------------------------------
    void loadEnvironment(ProjectPersistence::CMakeBuildProfile* target)
    {

    }
//#####################################################################################################################
}
