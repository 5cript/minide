#pragma once

#include "cmake_project_settings.hpp"

namespace MinIDE
{
    struct Settings
    {
        CMakeProjectSettings cmakeProjectSettings;
        std::string toolDirectory = "D:/msys2/mingw64/bin";
        std::string cmakeCommand = "cmake.exe";
        std::string makeCommand = "mingw32-make.exe";
    };
}
