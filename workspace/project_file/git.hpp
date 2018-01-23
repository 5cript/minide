#pragma once

#include "../../json_public.hpp"

#include <string>
#include <boost/optional.hpp>

namespace MinIDE::ProjectPersistence
{
    struct Git : public JSON::Stringifiable <Git>
                       , public JSON::Parsable <Git>
    {
        std::string origin;

    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::Git,
    origin
)
