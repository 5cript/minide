#include "environment_options.hpp"
#include "environment_options/listbox_inline.hpp"

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
        EnvironmentOptionsImpl(nana::window owner, GlobalPersistence* settings);

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
        GlobalPersistence* settings;

        // Other
        std::string lastSelectedProfile;
        bool dirty; // dirty = some changes were made
    };
//---------------------------------------------------------------------------------------------------------------------
    EnvironmentOptionsImpl::EnvironmentOptionsImpl(nana::window owner, GlobalPersistence* settings)
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
        , settings{settings}
        , lastSelectedProfile{""}
        , dirty{false}
    {
    }
//#####################################################################################################################
    EnvironmentOptions::EnvironmentOptions(nana::window owner, GlobalPersistence* settings)
        : elements_{new EnvironmentOptionsImpl(owner, settings)}
    {
        setupEnvironmentListbox();
        setupLayout();
        setupEvents();
        loadEnvironments();
    }
//---------------------------------------------------------------------------------------------------------------------
    void EnvironmentOptions::loadEnvironments()
    {
        GlobalPersistence* settings = elements_->settings;
        for (auto const& i : settings->environments)
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

                auto env = elements_->settings->environments.find(text.value());
                if (env != std::end(elements_->settings->environments))
                {
                    nana::msgbox box(elements_->form, "Already exists", nana::msgbox::ok);
                    box.icon(nana::msgbox::icon_error) << "The environment profile already exists.";
                    box.show();
                    return;
                }
                else if (!useWizard.value())
                {
                    elements_->settings->environments[text.value()] = {};
                    elements_->settings->environments[text.value()].name = text.value();

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
                auto prof = elements_->settings->environments.find(elements_->lastSelectedProfile);
                if (prof != std::end(elements_->settings->environments))
                {
                    elements_->settings->environments.erase(prof);
                }
                elements_->profileSelector.erase(elements_->profileSelector.option());
                disableAllOrSelect();
                elements_->settings->save();
            }
        });

        elements_->addEnvironmentVar.events().click([this](auto const& event)
        {
            elements_->envVars.at(0).append({"ENTER_KEY", "ENTER_VALUE"});
        });

        elements_->removeEnvironmentVar.events().click([this](auto const& event)
        {

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
        auto& environment = elements_->settings->environments[elements_->lastSelectedProfile];
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
                auto& environment = elements_->settings->environments[elements_->lastSelectedProfile];
                environment.path = formatPath(elements_->pathBox.caption(), true);
                elements_->settings->save();
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
        layout.field("EnvironmentList") << elements_->envVars;
        layout.field("AddEnv") << elements_->addEnvironmentVar;
        layout.field("RemoveEnv") << elements_->removeEnvironmentVar;
        layout.field("RemoveProfileButton") << elements_->removeProfile;
        layout.field("PathLabel") << elements_->pathLabel;
        layout.field("PathBox") << elements_->pathBox;

        layout.div(layoutString);
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string EnvironmentOptions::formatPath(std::string const& path, bool toMachine)
    {
        std::string result;
        if (!toMachine)
        {
            for (auto const i : path)
            {
                if (i == ';')
                {
                    result.push_back(';');
                    result.push_back('\n');
                }
                else
                    result.push_back(i);
            }
        }
        else
        {
            bool trimEnabled = false;
            for (auto const i : path)
            {
                if (trimEnabled)
                {
                    if (std::isspace(i))
                        continue;
                    else
                        trimEnabled = false;
                }
                if (i == ';')
                    trimEnabled = true;
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

        elements_->envVars.at(0).inline_factory(0, nana::pat::make_factory<EditableListElement>());
        elements_->envVars.at(0).inline_factory(1, nana::pat::make_factory<EditableListElement>());
    }
//---------------------------------------------------------------------------------------------------------------------
    EnvironmentOptions::~EnvironmentOptions() = default;
//#####################################################################################################################
}
