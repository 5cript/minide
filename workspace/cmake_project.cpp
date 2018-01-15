#include "cmake_project.hpp"
#include "globber.hpp"

#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <stdexcept>

using namespace std::string_literals;

namespace MinIDE
{
//#####################################################################################################################
    CMakeProject::CMakeProject(Settings* settings, path const& rootDir)
        : Project{settings}
    {
        loadEnvironment(rootDir);
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::loadEnvironment(path const& rootDir)
    {
        rootDir_ = rootDir;

        std::cout << rootDir.string() << "\n";

        // Should never be possible to throw. Safeguard.
        if (!filesystem::exists(rootDir / "CMakeLists.txt"))
            throw std::invalid_argument("This is not a CMake directory");

        glob(rootDir, settings_->cmakeProjectSettings.globbing.masks, settings_->cmakeProjectSettings.globbing.dirBlacklist);
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::buildStep(int step, bool debug)
    {
        if (step == 0)
            runCMake(debug);
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::runCMake(bool debug)
    {
        auto dir = rootDir_;
        if (debug)
            dir /= settings_->cmakeProjectSettings.debugOutputDir;
        else
            dir /= settings_->cmakeProjectSettings.releaseOutputDir;

        filesystem::create_directory(dir);

        process_ = std::make_unique <AsyncProcess> (
            settings_->toolDirectory
                + "/"
                + settings_->cmakeCommand
                + " \""s
                + rootDir_.string()
                + "\" "
                + settings_->cmakeProjectSettings.cmakeParameters,
            dir.string(),
            cb_
        );
    }
//#####################################################################################################################
}
