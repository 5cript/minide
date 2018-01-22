#pragma once

#include "globber_settings.hpp"
#include "../json_public.hpp"

namespace MinIDE
{
    struct CMakeBaseSettings : public JSON::Stringifiable <CMakeBaseSettings>
                             , public JSON::Parsable <CMakeBaseSettings>
    {
        GlobSettings globbing;
        std::string debugOutputDir = "build_debug";
        std::string releaseOutputDir = "build";
        std::string cmakeParameters = "-G\"MSYS Makefiles\" -D\"CMAKE_MAKE_PROGRAM:PATH=D:/msys2/mingw64/bin/mingw32-make.exe\"";
        std::string makeParameters = "-j4";
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::CMakeBaseSettings,
    globbing, debugOutputDir, releaseOutputDir, cmakeParameters, makeParameters
)
