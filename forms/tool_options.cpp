#include "tool_options.hpp"

#include "../resources.hpp"

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/msgbox.hpp>

namespace MinIDE
{
//#####################################################################################################################
    struct ToolOptionsImpl
    {
        ToolOptionsImpl(nana::window owner, GlobalPersistence* settings);

        // Form
        nana::form form;
        GlobalPersistence* settings;

        // Widgets
        nana::button addProfile;
        nana::button removeProfile;

        nana::combox profileSelector;

        nana::label cmakeLabel;
        nana::textbox cmakeBox;

        nana::label makeLabel;
        nana::textbox makeBox;

        nana::label shellLabel;
        nana::textbox shellBox;

        // Layout
        nana::place layout;

        // Other
        bool dirty;
        std::string lastSelectedProfile;
    };
//---------------------------------------------------------------------------------------------------------------------
    ToolOptionsImpl::ToolOptionsImpl(nana::window owner, GlobalPersistence* settings)
        : form{owner, nana::API::make_center(owner, 500, 140)}
        , settings{settings}
        , addProfile{form, "Add"}
        , removeProfile{form, "Remove"}
        , profileSelector{form}
        , cmakeLabel{form, "cmake: "}
        , cmakeBox{form, "cmake.exe"}
        , makeLabel{form, "make: "}
        , makeBox{form, "mingw32-make.exe"}
        , shellLabel{form, "shell: "}
        , shellBox{form, "bash.exe"}
        , layout{form}
        , dirty{false}
        , lastSelectedProfile{}
    {
    }
//#####################################################################################################################
    ToolOptions::ToolOptions(nana::window owner, GlobalPersistence* settings)
        : elements_{new ToolOptionsImpl(owner, settings)}
    {
        setupLayout();
        setupEvents();
        loadProfiles();
    }
//---------------------------------------------------------------------------------------------------------------------
    void ToolOptions::loadProfiles()
    {
        GlobalPersistence* settings = elements_->settings;
        for (auto const& i : settings->tooling)
            elements_->profileSelector.push_back(i.first);

        disableAllOrSelect();
    }
//---------------------------------------------------------------------------------------------------------------------
    void ToolOptions::setupLayout()
    {
        auto& layout = elements_->layout;

        layout.field("ProfileComboBox") << elements_->profileSelector;
        layout.field("AddProfileButton") << elements_->addProfile;
        layout.field("RemoveProfileButton") << elements_->removeProfile;

        layout.field("CMakeLabel") << elements_->cmakeLabel;
        layout.field("CMakeBox") << elements_->cmakeBox;
        layout.field("MakeLabel") << elements_->makeLabel;
        layout.field("MakeBox") << elements_->makeBox;
        layout.field("ShellLabel") << elements_->shellLabel;
        layout.field("ShellBox") << elements_->shellBox;

        layout.div(loadResource("layouts/tool_options.layout"));
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void ToolOptions::setupEvents()
    {
        elements_->addProfile.events().click([this](auto const& event)
        {
            nana::inputbox box{elements_->form, "Input a profile name", "Enter a name"};
            nana::inputbox::text text{"Name: ", "NewProfile"};
            if (box.show_modal(text))
            {
                auto env = elements_->settings->tooling.find(text.value());
                if (env != std::end(elements_->settings->tooling))
                {
                    nana::msgbox box(elements_->form, "Already exists", nana::msgbox::ok);
                    box.icon(nana::msgbox::icon_error) << "The tool profile already exists.";
                    box.show();
                    return;
                }
                else
                {
                    elements_->settings->tooling[text.value()] = {};

                    elements_->profileSelector.push_back(text.value());
                    elements_->profileSelector.option(elements_->profileSelector.the_number_of_options() - 1);
                    elements_->dirty = true;
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
                auto prof = elements_->settings->tooling.find(elements_->lastSelectedProfile);
                if (prof != std::end(elements_->settings->tooling))
                {
                    elements_->settings->tooling.erase(prof);
                }
                elements_->profileSelector.erase(elements_->profileSelector.option());
                disableAllOrSelect();
                elements_->settings->save();
                elements_->dirty = false;
            }
        });

        elements_->profileSelector.events().selected([this](auto const& event)
        {
            promptSave();
            elements_->lastSelectedProfile = event.widget.caption();
            loadProfile();
            enableAll();
        });

        elements_->form.events().unload([this](auto const& event)
        {
            promptSave();
        });

        elements_->cmakeBox.events().text_changed([this](auto const& event){elements_->dirty = true;});
        elements_->makeBox.events().text_changed([this](auto const& event){elements_->dirty = true;});
        elements_->shellBox.events().text_changed([this](auto const& event){elements_->dirty = true;});
    }
//---------------------------------------------------------------------------------------------------------------------
    void ToolOptions::loadProfile()
    {
        auto& tools = elements_->settings->tooling[elements_->lastSelectedProfile];

        elements_->cmakeBox.caption(tools.cmake);
        elements_->makeBox.caption(tools.make);
        elements_->shellBox.caption(tools.shell);

        elements_->dirty = false;
    }
//---------------------------------------------------------------------------------------------------------------------
    void ToolOptions::promptSave()
    {
        if (!elements_->lastSelectedProfile.empty() && elements_->dirty)
        {
            nana::msgbox box(nullptr, "Data Loss Possible", nana::msgbox::yes_no);
            box.icon(box.icon_warning);
            box << "Do you want to save the changes first?";
            auto result = box.show();
            if (result == nana::msgbox::pick_yes)
            {
                auto& tools = elements_->settings->tooling[elements_->lastSelectedProfile];
                tools.cmake = elements_->cmakeBox.caption();
                tools.make = elements_->makeBox.caption();
                tools.shell = elements_->shellBox.caption();
                elements_->settings->save();
            }
            elements_->dirty = false;
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void ToolOptions::disableAllOrSelect()
    {
        if (elements_->profileSelector.the_number_of_options() > 0)
            elements_->profileSelector.option(0);
        else
        {
            elements_->cmakeBox.caption("");
            elements_->makeBox.caption("");
            elements_->shellBox.caption("");

            elements_->cmakeBox.enabled(false);
            elements_->makeBox.enabled(false);
            elements_->shellBox.enabled(false);
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void ToolOptions::enableAll()
    {
        elements_->cmakeBox.enabled(true);
        elements_->makeBox.enabled(true);
        elements_->shellBox.enabled(true);
    }
//---------------------------------------------------------------------------------------------------------------------
    void ToolOptions::show()
    {
        nana::API::modal_window(elements_->form);
        elements_->form.show();
    }
//---------------------------------------------------------------------------------------------------------------------
    ToolOptions::~ToolOptions() = default;
//#####################################################################################################################
}
