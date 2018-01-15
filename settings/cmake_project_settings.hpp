#pragma once

#include "globSettings.hpp"

namespace MinIDE
{
    struct CMakeProjectSettings
    {
        GlobSettings globbing;
        std::string debugOutputDir = "build_debug";
        std::string releaseOutputDir = "build";
        std::string cmakeParameters = "-G\"MSYS Makefiles\"";
        std::string makeParameters = "-j4";
    };
}
