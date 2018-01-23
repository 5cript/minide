#pragma once

#include "../../json_public.hpp"

#include <string>
#include <boost/optional.hpp>

namespace MinIDE::ProjectPersistence
{
    struct BuildProfile : public JSON::Stringifiable <BuildProfile>
                        , public JSON::Parsable <BuildProfile>
    {
        std::string name;
        std::string outputPath;
        boost::optional <std::string> executable;
        bool isDebugable;

        virtual ~BuildProfile() = default;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::BuildProfile,
    name, outputPath, executable, isDebugable
)
