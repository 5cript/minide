#pragma once

#include "../json_public.hpp"

#include <vector>
#include <string>

namespace MinIDE
{
    struct GlobSettings : public JSON::Stringifiable <GlobSettings>
                        , public JSON::Parsable <GlobSettings>
    {
        std::vector <std::string> masks = {"*.cpp", "*.hpp", "*.h", "*.c", "CMakeLists.txt"};
        std::vector <std::string> dirBlacklist = {".git", "build"};
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::GlobSettings,
    masks, dirBlacklist
)
