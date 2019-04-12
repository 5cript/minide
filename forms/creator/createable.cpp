#include "createable.hpp"
#include "../../scripting_engine/api/wizard.hpp"

#include <boost/algorithm/string.hpp>

#include <string>
#include <vector>

namespace MinIDE
{
//#####################################################################################################################
    Creatable::Creatable(std::string name, std::string description, Scripting::Script script, std::optional <std::string> imageResource)
        : name_{std::move(name)}
        , description_{std::move(description)}
        , script_{std::move(script)}
        , imageResource_{std::move(imageResource)}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Creatable::description() const
    {
        return description_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Creatable::name() const
    {
        return name_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::optional <std::string> Creatable::imageResource() const
    {
        return imageResource_;
    }
//---------------------------------------------------------------------------------------------------------------------
    Scripting::Script const& Creatable::script() const
    {
        return script_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Creatable::catenated() const
    {
        std::string cat;
        if (imageResource_)
            cat = imageResource_.value() + ";";
        cat += name_ + ";";
        cat += description_ + ";";
        cat += script_.script();

        return cat;
    }
//---------------------------------------------------------------------------------------------------------------------
    Creatable Creatable::splitParse(std::string const& str)
    {
        if (str.empty())
            return {};

        std::vector<std::string> strs;
        boost::split(strs, str, boost::is_any_of(";"));

        if (strs.empty())
            return {};

        if (strs.size() == 2)
            return {strs[0], strs[1], {}, std::nullopt};
        else if (strs.size() == 3)
            return {strs[1], strs[2], {}, strs[0]};
        else if (strs.size() == 4)
            return {strs[1], strs[2], strs[3], strs[0]};

        return {};
    }
//---------------------------------------------------------------------------------------------------------------------
    void Creatable::startWizard()
    {
        using namespace Scripting::Api;

        Wizard wizard{&script_};
        auto parameters = wizard.retrieveParameters();
    }
//#####################################################################################################################
}
