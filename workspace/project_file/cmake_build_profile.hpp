#pragma once

#include "build_profile.hpp"

namespace MinIDE::ProjectPersistence
{
    struct CMakeBuildProfile : BuildProfile
                             , public JSON::Stringifiable <CMakeBuildProfile>
                             , public JSON::Parsable <CMakeBuildProfile>

    {
        CMakeBuildProfile(
            std::string name = "",
            std::string outputPath = "",
            std::string environment = "",
            std::string toolProfile = "",
            boost::optional <std::string> executable = boost::none,
            bool isDebugable = false,
            boost::optional <std::string> cmakeOptions = boost::none
        );

        boost::optional <std::string> cmakeOptions;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::CMakeBuildProfile,
    name, outputPath, executable, isDebugable, environment, toolProfile, cmakeOptions
)
