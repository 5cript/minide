#pragma once

#include "../../json_public.hpp"

#include <string>
#include <boost/optional.hpp>

namespace MinIDE::ProjectPersistence
{
    struct BuildProfile // NO INHERITANCE
    {
        virtual ~BuildProfile() = default;
        BuildProfile(
            std::string name = "",
            std::string outputPath = "",
            std::string environment = "",
            std::string toolProfile = "",
            boost::optional <std::string> executable = boost::none,
            bool isDebugable = false
        );

        std::string name;
        std::string outputPath;
        std::string environment;
        std::string toolProfile;
        boost::optional <std::string> executable;
        bool isDebugable;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::BuildProfile,
    name, outputPath, executable, isDebugable, environment, toolProfile
)
