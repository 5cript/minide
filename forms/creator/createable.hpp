#pragma once

#include "../../scripting_engine/script.hpp"
#include "../../filesystem.hpp"

#ifndef Q_MOC_RUN // A Qt workaround, for those of you who use Qt
#   include <SimpleJSON/parse/jsd.hpp>
#   include <SimpleJSON/stringify/jss.hpp>
#   include <SimpleJSON/stringify/jss_fusion_adapted_struct.hpp>
#endif

#include <string>
#include <optional>

namespace MinIDE
{
    struct SerializableCreatable : public JSON::Stringifiable <SerializableCreatable>
                                 , public JSON::Parsable <SerializableCreatable>
    {
        std::string name;
        std::string description;
        std::string script;
        std::optional <std::string> image;
    };

    struct SerializableCreatables : public JSON::Stringifiable <SerializableCreatables>
                                  , public JSON::Parsable <SerializableCreatables>
    {
        std::vector <SerializableCreatable> creatables;
    };

    class Creatable
    {
    public:
        enum class WantsToCreate
        {
            File,
            Folder
        };

    public:
        Creatable() = default;
        ~Creatable() = default;
        Creatable(std::string name, std::string description, Scripting::Script script, std::optional <std::string> imageResource);
        Creatable(SerializableCreatable const& seriCreate);

        static Creatable splitParse(std::string const& str);

        std::string description() const;
        std::string name() const;
        std::optional <std::string> imageResource() const;

        /**
         *  For displaying in the listbox
         */
        std::string catenated() const;
        Scripting::Script const& script() const;

        /**
         *  Returns the path the project was created in on success.
         **/
        std::optional <path> startWizard();

    private:
        std::string name_;
        std::string description_;
        Scripting::Script script_;
        std::optional <std::string> imageResource_;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::SerializableCreatable,
    name, description, script, image
)

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::SerializableCreatables,
    creatables
)
