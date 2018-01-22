#pragma once

#include "../json_public.hpp"

namespace MinIDE
{
    struct Tools : public JSON::Stringifiable <Tools>
                 , public JSON::Parsable <Tools>
    {
        std::string cmake = "cmake.exe";
        std::string make = "mingw32-make.exe";
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::Tools,
    cmake, make
)
