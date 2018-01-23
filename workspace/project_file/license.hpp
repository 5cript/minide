#pragma once

#include "../../json_public.hpp"

#include <string>
#include <boost/optional.hpp>

namespace MinIDE::ProjectPersistence
{
    struct License : public JSON::Stringifiable <License>
                   , public JSON::Parsable <License>
    {
        std::string type;
        boost::optional <std::string> url;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::License,
    type, url
)
