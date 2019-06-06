#include "environment_options.hpp"
#include "environment_options/listbox_inline.hpp"

#include "../resources.hpp"

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/msgbox.hpp>

#include <iostream>
#include <cctype>

using namespace std::string_literals;

namespace MinIDE
{
//#####################################################################################################################
    struct EnvironmentOptionsImpl
    {
        EnvironmentOptionsImpl(
            nana::window owner,
            std::unordered_map <std::string, Environment>* environments,
            std::function <void()> saveFunction,
            bool noProfiles
        );

        // Form
        nana::form form;

        // Widgets
        nana::listbox envVars;
        nana::button addProfile;
        nana::button removeProfile;
        nana::combox profileSelector;
        nana::button removeEnvironmentVar;
        nana::button addEnvironmentVar;
        nana::label pathLabel;
        nana::textbox pathBox;

        // Layout
        nana::place layout;

        // Persistence Related
        std::unordered_map <std::string, Environment>* environments;
        std::function <void()> saveFunction;

        // Other
        std::string lastSelectedProfile;
        bool dirty; // dirty = some changes were made
        bool noProfiles;
    };
//---------------------------------------------------------------------------------------------------------------------
    EnvironmentOptionsImpl::EnvironmentOptionsImpl(
        nana::window owner,
        std::unordered_map <std::string, Environment>* environments,
        std::function <void()> saveFunction,
        bool noProfiles
    )
        : form{owner, nana::API::make_center(owner, 600, 400)}
        , envVars{form}
        , addProfile{form, "Add Profile"}
        , removeProfile{form, "Delete Profile"}
        , profileSelector{form}
        , removeEnvironmentVar{form, "Remove Variable"}
        , addEnvironmentVar{form, "Add Variable"}
        , pathLabel{form, "Path"}
        , pathBox{form}
        , layout{form}
        , environments{environments}
        , saveFunction{saveFunction}
        , lastSelectedProfile{""}
        , dirty{false}
        , noProfiles{noProfiles}
    {
        form.caption("Environment Variables");
    }
//#####################################################################################################################
    EnvironmentOptions::EnvironmentOptions(
        nana::window owner,
        std::unordered_map <std::string, Environment>* environments,
        std::function <void()> saveFunction,
        bool noProfiles
    )
        : elements_{new EnvironmentOptionsImpl(owner, environments, saveFunction, noProfiles)}
    {
        setupEnvironmentListbox();
        setupLayout();
        setupEvents();
        loadEnvironments();
    }
//---------------------------------------------------------------------------------------------------------------------
    void EnvironmentOptions::loadEnvironments()
    {
        for (auto const& i : *elements_->environments)
            elements_->profileSelector.push_back(i.first);

        disableAllOrSelect();
    }
//---------------------------------------------------------------------------------------------------------------------
    void EnvironmentOptions::show()
    {
        nana::API::modal_window(elements_->form);
        elements_->form.show();
    }
//---------------------------------------------------------------------------------------------------------------------
    void EnvironmentOptions::setupEvents()
    {
        elements_->addProfile.events().click([this](auto const& event)
        {
            nana::inputbox box{elements_->form, "Input a profile name", "Enter a name"};
            nana::inputbox::text text{"Name: ", "NewProfile"};
            nana::inputbox::boolean useWizard{"Use Wizard", false};
            if (box.show_modal(text, useWizard))
            {
                //std::cout << text.value() << "\n";

                auto env = elements_->environments->find(text.value());
                if (env != std::end(*elements_->environments))
                {
                    nana::msgbox box(elements_->form, "Already exists", nana::msgbox::ok);
                    box.icon(nana::msgbox::icon_error) << "The environment profile already exists.";
                    box.show();
                    return;
                }
                else if (!useWizard.value())
                {
                    (*elements_->environments)[text.value()] = {};
                    (*elements_->environments)[text.value()].name = text.value();

                    elements_->profileSelector.push_back(text.value());
                    elements_->profileSelector.option(elements_->profileSelector.the_number_of_options() - 1);
                    elements_->dirty = true;
                }
                else if (useWizard.value())
                {
                    nana::msgbox(elements_->form, "Wizard not implemented", nana::msgbox::ok).icon(nana::msgbox::icon_error)
                        << "Wizard not implemented";
                    return;
                }
            }
        });

        elements_->removeProfile.events().click([this](auto const& event)
        {
            nana::msgbox box(nullptr, "Data Loss Possible", nana::msgbox::yes_no);
            box.icon(box.icon_warning);
            box << "Do you want to remove this profile?";
            auto result = box.show();
            if (result == nana::msgbox::pick_yes)
            {
                auto prof = elements_->environments->find(elements_->lastSelectedProfile);
                if (prof != std::end(*elements_->environments))
                {
                    elements_->environments->erase(prof);
                }
                elements_->profileSelector.erase(elements_->profileSelector.option());
                disableAllOrSelect();
                elements_->saveFunction();
            }
        });

        elements_->addEnvironmentVar.events().click([this](auto const& event)
        {
            elements_->envVars.at(0).append({"ENTER_KEY", "ENTER_VALUE"});
            elements_->dirty = true;
        });

        elements_->removeEnvironmentVar.events().click([this](auto const& event)
        {
            elements_->dirty = true;
        });

        elements_->form.events().unload([this](auto const& event)
        {
            promptSave();
        });

        elements_->profileSelector.events().selected([this](auto const& event)
        {
            promptSave();
            elements_->lastSelectedProfile = event.widget.caption();
            loadProfile();
            enableAll();
        });

        elements_->pathBox.events().text_changed([this](auto const& event)
        {
            elements_->dirty = true;
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void EnvironmentOptions::disableAllOrSelect()
    {
        if (elements_->profileSelector.the_number_of_options() > 0)
            elements_->profileSelector.option(0);
        else
        {
            elements_->pathBox.caption("");
            elements_->envVars.clear();

            elements_->pathBox.enabled(false);
            elements_->envVars.enabled(false);
            elements_->removeProfile.enabled(false);
            elements_->removeEnvironmentVar.enabled(false);
            elements_->addEnvironmentVar.enabled(false);
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void EnvironmentOptions::enableAll()
    {
        elements_->pathBox.enabled(true);
        elements_->envVars.enabled(true);
        elements_->removeProfile.enabled(true);
        //elements_->removeEnvironmentVar.enabled(true);
        //elements_->addEnvironmentVar.enabled(true);
    }
//---------------------------------------------------------------------------------------------------------------------
    void EnvironmentOptions::loadProfile()
    {
        auto& environment = (*elements_->environments)[elements_->lastSelectedProfile];
        elements_->pathBox.caption(formatPath(environment.path, false));

        elements_->envVars.clear();
        for (auto const& variable : environment.variables)
            elements_->envVars.at(0).append({variable.first, variable.second});

        elements_->dirty = false;
    }
//---------------------------------------------------------------------------------------------------------------------
    void EnvironmentOptions::promptSave()
    {
        if (!elements_->lastSelectedProfile.empty() && elements_->dirty)
        {
            nana::msgbox box(nullptr, "Data Loss Possible", nana::msgbox::yes_no);
            box.icon(box.icon_warning);
            box << "Do you want to save the changes first?";
            auto result = box.show();
            if (result == nana::msgbox::pick_yes)
            {
                auto& environment = (*elements_->environments)[elements_->lastSelectedProfile];
                environment.path = formatPath(elements_->pathBox.caption(), true);

                auto envVarCount = elements_->envVars.size_item(0);
                std::unordered_map <std::string, std::string> vars;
                for (decltype(envVarCount) i = decltype(envVarCount){}; i != envVarCount; ++i)
                {
                    std::cout << elements_->envVars.at(0).at(i).text(0) << "=" << elements_->envVars.at(0).at(i).text(1) << "\n";
                    vars[elements_->envVars.at(0).at(i).text(0)] = elements_->envVars.at(0).at(i).text(1);
                }
                environment.variables = vars;

                elements_->saveFunction();
            }
            elements_->dirty = false;
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void EnvironmentOptions::setupLayout()
    {
        auto& layout = elements_->layout;

        layout.field("ProfileComboBox") << elements_->profileSelector;
        layout.field("AddProfileButton") << elements_->addProfile;
        layout.field("RemoveProfileButton") << elements_->removeProfile;
        layout.field("EnvironmentList") << elements_->envVars;
        layout.field("AddEnv") << elements_->addEnvironmentVar;
        layout.field("RemoveEnv") << elements_->removeEnvironmentVar;
        layout.field("PathLabel") << elements_->pathLabel;
        layout.field("PathBox") << elements_->pathBox;

        layout.div(loadResource("layouts/environment_options.layout"));
        if (elements_->noProfiles)
            layout.field_display("ProfileChooserPane", false);
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string EnvironmentOptions::formatPath(std::string const& path, bool toMachine)
    {
        std::string result;
        if (!toMachine) // for human
        {
            for (auto const i : path)
            {
                if (i == ';')
                {
                    result.push_back('\n');
                }
                else
                    result.push_back(i);
            }
        }
        else // for machine
        {
            bool trimEnabled = false;
            for (auto const i : path)
            {
                if (trimEnabled)
                {
                    if (std::isspace(i))
                        continue;
                    else
                    {
                        result.push_back(';');
                        trimEnabled = false;
                    }
                }
                if (i == '\n')
                    trimEnabled = true;
                else
                    result.push_back(i);
            }
        }
        return result;
    }
//---------------------------------------------------------------------------------------------------------------------
    void EnvironmentOptions::setupEnvironmentListbox()
    {
        elements_->envVars.append_header("Variable", 90);
        elements_->envVars.append_header("Value", 480);

        elements_->envVars.at(0).inline_factory(0, nana::pat::make_factory<EditableListElement>(&elements_->dirty));
        elements_->envVars.at(0).inline_factory(1, nana::pat::make_factory<EditableListElement>(&elements_->dirty));
    }
//---------------------------------------------------------------------------------------------------------------------
    EnvironmentOptions::~EnvironmentOptions() = default;
//#####################################################################################################################
}
