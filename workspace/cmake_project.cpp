#include "cmake_project.hpp"
#include "globber.hpp"

#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <stdexcept>

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
//#####################################################################################################################
}
