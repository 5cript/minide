#pragma once

#include "../json_public.hpp"

#include <unordered_map>
#include <string>

namespace MinIDE
{
    struct Environment : public JSON::Stringifiable <Environment>
                       , public JSON::Parsable <Environment>
    {
        std::string name;
        std::string path;
        std::unordered_map <std::string, std::string> variables;

        std::unordered_map <std::string, std::string> compile() const;

        /// priority is other, if clashing
        Environment merge(Environment const& other);
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::Environment,
    name, path, variables
)
