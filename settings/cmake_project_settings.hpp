#pragma once

#include "globSettings.hpp"

namespace MinIDE
{
    struct CMakeProjectSettings
    {
        GlobSettings globbing;
        std::string debugOutputDir = "build_debug";
        std::string releaseOutputDir = "build";
        std::string cmakeParameters = "-G\"MSYS Makefiles\" -D\"CMAKE_MAKE_PROGRAM:PATH=D:/msys2/mingw64/bin/mingw32-make.exe\"";
        std::string makeParameters = "-j4";
    };
}
