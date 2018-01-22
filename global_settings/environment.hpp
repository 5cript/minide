#pragma once

#include "../json_public.hpp"

#include <unordered_map>
#include <string>

namespace MinIDE
{
    struct Environment : public JSON::Stringifiable <Environment>
                       , public JSON::Parsable <Environment>
                       , public Serializable
    {
        SERIALIZABLE;

        std::string name;
        std::string path;
        std::unordered_map <std::string, std::string> variables;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::Environment,
    name, path, variables
)
