#pragma once

#include <vector>
#include <string>

namespace MinIDE
{
    struct GlobSettings
    {
        std::vector <std::string> masks = {"*.cpp", "*.hpp", "*.h", "*.c", "CMakeLists.txt"};
        std::vector <std::string> dirBlacklist = {".git", "build"};
    };
}
