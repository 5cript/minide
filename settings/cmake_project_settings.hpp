#pragma once

namespace MinIDE
{
    struct CMakeProjectSettings
    {
        std::vector <std::string> projectGlobFilter = {"*.cpp", "*.hpp", "*.h", "*.c", "CMakeLists.txt"};
    };
}
