#pragma once

#include "build_profile.hpp"

namespace MinIDE::ProjectPersistence
{
    struct CmakeBuildProfile : BuildProfile
                             , public JSON::Stringifiable <CmakeBuildProfile>
                             , public JSON::Parsable <CmakeBuildProfile>

    {
        boost::optional <std::string> CmakeOptions;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::CmakeBuildProfile,
    CmakeOptions
)
