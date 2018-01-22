#include "cmake_project.hpp"
#include "globber.hpp"

#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <stdexcept>
#include <cstdio>

using namespace std::string_literals;

namespace MinIDE
{
//#####################################################################################################################
    CMakeProject::CMakeProject(GlobalPersistence* settings, std::string* currentEnvironment, path const& rootDir)
        : Project{settings, currentEnvironment}
    {
        load(rootDir);
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::load(path const& rootDir)
    {
        /*
        rootDir_ = rootDir;

        std::cout << rootDir.string() << "\n";

        // Should never be possible to throw. Safeguard.
        if (!filesystem::exists(rootDir / "CMakeLists.txt"))
            throw std::invalid_argument("This is not a CMake directory");

        glob(rootDir, settings_->cmakeProjectSettings.globbing.masks, settings_->cmakeProjectSettings.globbing.dirBlacklist);

        _putenv(("PATH="s + environment_->path).c_str());
        */
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::buildStep(int step, bool debug)
    {
        switch (step)
        {
        case 0:
            runCMake(debug);
            break;
        case 1:
            runMake(debug);
            break;
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::runCMake(bool debug)
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
    void CMakeProject::runMake(bool debug)
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
    path CMakeProject::buildDir(bool debug) const
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
    void CMakeProject::run(bool debug)
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
//#####################################################################################################################
}
