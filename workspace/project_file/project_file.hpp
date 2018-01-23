#pragma once

#include "git.hpp"
#include "license.hpp"
#include "../../json_public.hpp"

#include <string>
#include <vector>
#include <boost/optional.hpp>

namespace MinIDE::ProjectPersistence
{
    struct ProjectFile : public JSON::Stringifiable <ProjectFile>
                       , public JSON::Parsable <ProjectFile>
                       , public Serializable
    {
        SERIALIZABLE;

        std::string name;
        std::string type;
        boost::optional <std::vector <std::string>> executables;
        boost::optional <std::string> version;
        boost::optional <std::string> description;
        boost::optional <Git> git;
        boost::optional <std::vector <License>> licenses;
        boost::optional <std::string> author;
        boost::optional <std::vector <std::string>> contributors;
        boost::optional <std::string> email;
        boost::optional <std::string> url;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::ProjectFile,
    name, type, executables, version, description, git, licenses, author, contributors, email, url
)
