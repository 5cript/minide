#include "createable.hpp"
#include "../../scripting_engine/api/wizard.hpp"

#include <nana/gui/widgets/form.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>

#include <boost/algorithm/string.hpp>
#include <fmt/core.h>

#include <string>
#include <vector>
#include <iostream>

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
    bool Creatable::startWizard()
    {
        using namespace Scripting::Api;

        Wizard wizard{&script_};
        auto parameters = wizard.retrieveParameters();

        nana::form inputForm{nana::API::make_center(700, 450)};
        nana::place layout{inputForm};
        nana::button ok{inputForm, "Ok"};
        nana::button cancel{inputForm, "Cancel"};

        std::vector <std::unique_ptr <nana::textbox>> boxes;
        std::vector <std::unique_ptr <nana::label>> labels;

        std::string layoutString = "<vertical <vertical margin=3\n";
        for (auto& [key, param] : parameters)
        {
            auto& [id, value, description] = param;
            std::unique_ptr <nana::textbox> box{new nana::textbox({inputForm, value})};
            std::unique_ptr <nana::label> lbl{new nana::label(inputForm, id)};

            box->events().text_changed([&value](auto const& tb)
            {
                value = tb.widget.caption();
            });
            boxes.push_back(std::move(box));
            labels.push_back(std::move(lbl));

            layout.field(id.c_str()) << *boxes.back();
            layout.field((id + "_LABEL").c_str()) << *labels.back();

            layoutString += fmt::format(R"(
                <max=25
                    <vertical max=100
                        <weight=3>
                        <{0}_LABEL>
                    >
                    <{0}>
                >
                <weight=3>
            )", id);
        }
        layoutString += R"(
                >
                <max=30
                    <>
                    <OkButton max=120 margin=3>
                    <CancelButton max=120 margin=3>
                >
            >
        )";

        bool okayed{false};
        ok.events().click([&okayed, &inputForm](auto const& btn)
        {
            okayed = true;
            inputForm.close();
        });

        cancel.events().click([&okayed, &inputForm](auto const& btn)
        {
            okayed = false;
            inputForm.close();
        });

        layout["OkButton"] << ok;
        layout["CancelButton"] << cancel;

        layout.div(layoutString);
        layout.collocate();

        inputForm.size({700u, static_cast <unsigned int> (parameters.size()) * 28 + 30 + 50});

        nana::API::modal_window(inputForm);
        inputForm.show();

        if (!okayed)
            return false;

        for (auto& [key, param] : parameters)
        {
            auto& [id, value, description] = param;
            std::cout << value << "\n";
        }

        return true;
    }
//#####################################################################################################################
}
