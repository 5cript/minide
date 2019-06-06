#pragma once

#include "../script_view.hpp"
#include "../../filesystem.hpp"

#include <unordered_map>

namespace MinIDE::Scripting::Api
{
    class Wizard : public ScriptView
    {
    public:
        using ScriptView::ScriptView;

        struct Parameter
        {
            std::string name;
            std::string prettyName;
            std::string defaultValue;
            std::string description;
            bool isOptional;
            int orderHint;
        };

        struct Creation
        {
            std::string name; // relative to base directory
            std::string content;
            std::string type; // file/directory
        };

        using Parameters = std::unordered_map <
            std::string,
            Parameter
        >;

        /**
         *  Is it a wizard that creates a single file, or multiple directories?
         */
        std::string retrieveType() const;

        /**
         *  Retrieve a list of parameters that has to be supplied.
         */
        Parameters retrieveParameters() const;

        /**
         *  Runs the wizard with the parameters and
         */
        std::vector <Creation> runWizard(Parameters const& params, path const& root);
    private:
    };
}
