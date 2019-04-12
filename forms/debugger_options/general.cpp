#include "general.hpp"

#include "../../resources.hpp"

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/checkbox.hpp>

namespace MinIDE::DebuggerOptionFrames
{
//#####################################################################################################################
    struct GeneralFrameImpl
    {
        GeneralFrameImpl(nana::panel <true>* panel, DebuggerSettings* settings);

        nana::panel <true>* panel;
        DebuggerSettings* settings;

        // Widgets
        nana::checkbox autoBuild;
        nana::checkbox autoRunBinary;
        nana::checkbox watchLocal;
        nana::checkbox watchFunctionArgs;

        // Layout
        nana::place layout;

        // Other
        bool dirty;
    };
//---------------------------------------------------------------------------------------------------------------------
    GeneralFrameImpl::GeneralFrameImpl(nana::panel <true>* panel, DebuggerSettings* settings)
        : panel{panel}
        , settings{settings}
        , autoBuild{*panel, "Automatically build before debugger starts"}
        , autoRunBinary{*panel, "Immediately run binary (no continue press at start required)"}
        , watchLocal{*panel, "Automatically watch local variables"}
        , watchFunctionArgs{*panel, "Automatically watch function arguments"}
        , layout{*panel}
        , dirty{false}
    {
    }
//#####################################################################################################################
    GeneralFrame::GeneralFrame(nana::window parent, DebuggerSettings* settings)
        : nana::panel<true>{parent}
        , elements_{new GeneralFrameImpl(this, settings)}
    {
        setLayout();
        enabled(false);
        setupEvents();
    }
//---------------------------------------------------------------------------------------------------------------------
    GeneralFrame::~GeneralFrame() = default;
//---------------------------------------------------------------------------------------------------------------------
    void GeneralFrame::enabled(bool enable)
    {
        elements_->autoBuild.enabled(enable);
        elements_->autoRunBinary.enabled(enable);
        elements_->watchLocal.enabled(enable);
        elements_->watchFunctionArgs.enabled(enable);
    }
//---------------------------------------------------------------------------------------------------------------------
    void GeneralFrame::setLayout()
    {
        auto& layout = elements_->layout;

        layout.field("AutoBuild") << elements_->autoBuild;
        layout.field("AutoRunBinary") << elements_->autoRunBinary;
        layout.field("WatchLocal") << elements_->watchLocal;
        layout.field("WatchFunctionArgs") << elements_->watchFunctionArgs;

        layout.div(loadResource("layouts/debugger_option_frames/general.layout"));
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void GeneralFrame::loadFromSettings()
    {
        elements_->autoBuild.check(elements_->settings->autoRun);
        elements_->autoRunBinary.check(elements_->settings->autoRun);
        elements_->watchLocal.check(elements_->settings->autoWatchLocals);
        elements_->watchFunctionArgs.check(elements_->settings->autoWatchFunctionArgs);
    }
//---------------------------------------------------------------------------------------------------------------------
    void GeneralFrame::setupEvents()
    {
        elements_->autoBuild.events().checked(
            [this](auto const& event)
            {
                elements_->settings->autoBuild = elements_->autoBuild.checked();
                elements_->dirty = true;
            }
        );
        elements_->autoRunBinary.events().checked(
            [this](auto const& event)
            {
                elements_->settings->autoRun = elements_->autoRunBinary.checked();
                elements_->dirty = true;
            }
        );
        elements_->watchLocal.events().checked(
            [this](auto const& event)
            {
                elements_->settings->autoWatchLocals = elements_->watchLocal.checked();
                elements_->dirty = true;
            }
        );
        elements_->watchFunctionArgs.events().checked(
            [this](auto const& event)
            {
                elements_->settings->autoWatchFunctionArgs = elements_->watchFunctionArgs.checked();
                elements_->dirty = true;
            }
        );
    }
//---------------------------------------------------------------------------------------------------------------------
    bool GeneralFrame::isDirty() const
    {
        return elements_->dirty;
    }
//---------------------------------------------------------------------------------------------------------------------
    void GeneralFrame::resetDirty()
    {
        elements_->dirty = false;
    }
//#####################################################################################################################
}
