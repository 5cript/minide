#include "cmake_project.hpp"

#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <stdexcept>

namespace MinIDE
{
//#####################################################################################################################
    CMakeProject::CMakeProject(path const& rootDir)
        : Project{}
    {
        loadEnvironment(rootDir);
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeProject::loadEnvironment(path const& rootDir)
    {
        std::cout << rootDir.string() << "\n";

        // Should never be possible to throw. Safeguard.
        if (!filesystem::exists(rootDir / "CMakeLists.txt"))
            throw std::invalid_argument("This is not a CMake directory");

        for(auto& entry : boost::make_iterator_range(filesystem::recursive_directory_iterator(rootDir), {}))
        {

        }
    }
//#####################################################################################################################
}
