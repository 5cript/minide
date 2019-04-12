#include "debugger_options.hpp"
#include "debugger_options/general.hpp"
#include "debugger_options/gdb.hpp"

#include "../resources.hpp"

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/msgbox.hpp>

namespace MinIDE
{
//#####################################################################################################################
    struct DebuggerOptionsImpl
    {
        DebuggerOptionsImpl(
            nana::window owner,
            std::unordered_map <std::string, DebuggerSettings>* settings,
            std::function <void()> saveFunction
        );

        // Form
        nana::form form;

        // Settings
        std::unordered_map <std::string, DebuggerSettings>* settings;
        DebuggerSettings currentlyEdited;

        // Widgets
        nana::combox profileSelector;
        nana::button addProfile;
        nana::button removeProfile;
        nana::tabbar <std::string> tabBar;

        // Sub Frames
        DebuggerOptionFrames::GeneralFrame generalOptions;
        DebuggerOptionFrames::GdbFrame gdbOptions;

        // Layout
        nana::place layout;

        // Other
        bool dirty;
        std::string lastSelectedProfile;
        std::function <void()> saveFunction;
    };
//---------------------------------------------------------------------------------------------------------------------
    DebuggerOptionsImpl::DebuggerOptionsImpl(
            nana::window owner,
            std::unordered_map <std::string, DebuggerSettings>* settings,
            std::function <void()> saveFunction
    )
        : form{owner, nana::API::make_center(owner, 700, 450)}
        , settings{settings}
        , currentlyEdited{}
        , profileSelector{form}
        , addProfile{form, "Add"}
        , removeProfile{form, "Remove"}
        , tabBar{form}
        , generalOptions{form, &currentlyEdited}
        , gdbOptions{form, &currentlyEdited}
        , layout{form}
        , dirty{false}
        , lastSelectedProfile{}
        , saveFunction{std::move(saveFunction)}
    {
        tabBar.append("General", generalOptions);
        tabBar.append("GDB", gdbOptions);
    }
//#####################################################################################################################
    DebuggerOptions::DebuggerOptions(
        nana::window owner,
        std::unordered_map <std::string, DebuggerSettings>* settings,
        std::function <void()> saveFunction
    )
        : elements_{new DebuggerOptionsImpl(owner, settings, std::move(saveFunction))}
    {
        setLayout();
        setupEvents();
        loadProfiles();
    }
//---------------------------------------------------------------------------------------------------------------------
    DebuggerOptions::~DebuggerOptions() = default;
//---------------------------------------------------------------------------------------------------------------------
    void DebuggerOptions::show()
    {
        nana::API::modal_window(elements_->form);
        elements_->form.show();
    }
//---------------------------------------------------------------------------------------------------------------------
    bool DebuggerOptions::isDirty()
    {
        return elements_->dirty || elements_->gdbOptions.isDirty() || elements_->generalOptions.isDirty();
    }
//---------------------------------------------------------------------------------------------------------------------
    void DebuggerOptions::setLayout()
    {
        auto& layout = elements_->layout;

        layout.field("ProfileComboBox") << elements_->profileSelector;
        layout.field("AddProfileButton") << elements_->addProfile;
        layout.field("RemoveProfileButton") << elements_->removeProfile;
        layout.field("TabBar") << elements_->tabBar;

        layout.field("ChildFrames").fasten(elements_->generalOptions)
                                   .fasten(elements_->gdbOptions);

        layout.div(loadResource("layouts/debugger_options.layout"));
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void DebuggerOptions::setupEvents()
    {
        elements_->addProfile.events().click([this](auto const& event)
        {
            if (isDirty() && !promptSave())
                return;

            nana::inputbox box{elements_->form, "Input a profile name", "Enter a name"};
            nana::inputbox::text text{"Name: ", "NewProfile"};
            if (box.show_modal(text))
            {
                auto env = elements_->settings->find(text.value());
                if (env != std::end(*elements_->settings))
                {
                    nana::msgbox box(elements_->form, "Already exists", nana::msgbox::ok);
                    box.icon(nana::msgbox::icon_error) << "The tool profile already exists.";
                    box.show();
                    return;
                }
                else
                {
                    (*elements_->settings)[text.value()] = {};
                    elements_->currentlyEdited = (*elements_->settings)[text.value()];

                    elements_->profileSelector.push_back(text.value());
                    elements_->profileSelector.option(elements_->profileSelector.the_number_of_options() - 1);
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
                auto prof = elements_->settings->find(elements_->lastSelectedProfile);
                if (prof != std::end(*elements_->settings))
                {
                    elements_->settings->erase(prof);
                }
                elements_->profileSelector.erase(elements_->profileSelector.option());
                disableAllOrSelect();
                elements_->saveFunction();
                resetDirty();
            }
        });

        elements_->profileSelector.events().selected([this](auto const& event)
        {
            promptSave(false);
            elements_->lastSelectedProfile = event.widget.caption();
            loadProfile();
            enableAll();
        });

        elements_->form.events().unload([this](auto const& event)
        {
            if (!promptSave())
                event.cancel = true;
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    bool DebuggerOptions::promptSave(bool hasCancel)
    {
        if (!elements_->lastSelectedProfile.empty() && isDirty())
        {
            auto type = nana::msgbox::yes_no;
            if (hasCancel)
                type = nana::msgbox::yes_no_cancel;
            nana::msgbox box(nullptr, "Data Loss Possible", type);
            box.icon(box.icon_warning);
            box << "Do you want to save the changes first?";
            auto result = box.show();
            if (hasCancel && result == nana::msgbox::pick_cancel)
                return false;
            if (result == nana::msgbox::pick_yes)
            {
                (*elements_->settings)[elements_->lastSelectedProfile] = elements_->currentlyEdited;
                elements_->saveFunction();
            }
            resetDirty();
        }
        return true;
    }
//---------------------------------------------------------------------------------------------------------------------
    void DebuggerOptions::loadProfile()
    {
        elements_->currentlyEdited = (*elements_->settings)[elements_->lastSelectedProfile];

        elements_->generalOptions.loadFromSettings();
        elements_->gdbOptions.loadFromSettings();

        resetDirty();
    }
//---------------------------------------------------------------------------------------------------------------------
    void DebuggerOptions::disableAllOrSelect()
    {
        if (elements_->profileSelector.the_number_of_options() > 0)
            elements_->profileSelector.option(0);
        else
        {
            elements_->generalOptions.enabled(false);
            elements_->gdbOptions.enabled(false);
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void DebuggerOptions::loadProfiles()
    {
        for (auto const& i : *elements_->settings)
            elements_->profileSelector.push_back(i.first);

        disableAllOrSelect();
    }
//---------------------------------------------------------------------------------------------------------------------
    void DebuggerOptions::enableAll()
    {
        elements_->generalOptions.enabled(true);
        elements_->gdbOptions.enabled(true);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DebuggerOptions::resetDirty()
    {
        elements_->generalOptions.resetDirty();
        elements_->gdbOptions.resetDirty();
        elements_->dirty = false;
    }
//#####################################################################################################################
}
