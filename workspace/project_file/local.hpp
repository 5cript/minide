#pragma once

#include "../../json_public.hpp"
#include "build_profiles.hpp"

#include <string>
#include <boost/optional.hpp>

namespace MinIDE::ProjectPersistence
{
    struct LocalPersistence : public JSON::Stringifiable <LocalPersistence>
                            , public JSON::Parsable <LocalPersistence>
                            , public Serializable
    {
        SERIALIZABLE;

        std::vector <std::unique_ptr <BuildProfile>> buildProfiles;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::LocalPersistence,
    buildProfiles
)
