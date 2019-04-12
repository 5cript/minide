#pragma once

#include "../script_view.hpp"

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
            std::string defaultValue;
            std::string description;
        };

        using Parameters = std::unordered_map <
            std::string,
            Parameter
        >;

        Parameters retrieveParameters();
    private:
    };
}
