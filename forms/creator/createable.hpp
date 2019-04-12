#pragma once

#include "../../scripting_engine/script.hpp"

#include <string>
#include <optional>

namespace MinIDE
{
    class Creatable
    {
    public:
        Creatable() = default;
        ~Creatable() = default;
        Creatable(std::string name, std::string description, Scripting::Script script, std::optional <std::string> imageResource);

        static Creatable splitParse(std::string const& str);

        std::string description() const;
        std::string name() const;
        std::optional <std::string> imageResource() const;

        /**
         *  For displaying in the listbox
         */
        std::string catenated() const;
        Scripting::Script const& script() const;

        void startWizard();

    private:
        std::string name_;
        std::string description_;
        Scripting::Script script_;
        std::optional <std::string> imageResource_;
    };
}
