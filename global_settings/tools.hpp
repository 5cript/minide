#pragma once

#include "../json_public.hpp"

namespace MinIDE
{
    struct Tools : public JSON::Stringifiable <Tools>
                 , public JSON::Parsable <Tools>
    {
        std::string cmake;
        std::string make;
        std::string shell;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::Tools,
    cmake, make, shell
)
