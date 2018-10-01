#include "gdb.hpp"

#include "../environment_options.hpp"

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/checkbox.hpp>

namespace MinIDE::DebuggerOptionFrames
{
//#####################################################################################################################
    struct GdbFrameImpl
    {
        GdbFrameImpl(nana::panel <true>* panel, DebuggerSettings* settings);

        nana::panel <true>* panel;
        DebuggerSettings* settings;

        // Widgets
        nana::label gdbPathLabel;
        nana::textbox gdbPath;

        nana::checkbox fullyReadSymbols;
        nana::checkbox neverReadSymbols;
        nana::checkbox returnChildResult;

        nana::checkbox initCommandFileCheck;
        nana::textbox initCommandFile;

        nana::checkbox commandFileCheck;
        nana::textbox commandFile;

        nana::checkbox ignoreHomeGdbInit;
        nana::checkbox ignoreAllGdbInit;

        nana::checkbox environmentOverwrites;
        nana::button changeEnvironment;

        // Layout
        nana::place layout;

        // Other
        bool dirty;
    };
//---------------------------------------------------------------------------------------------------------------------
    GdbFrameImpl::GdbFrameImpl(nana::panel <true>* panel, DebuggerSettings* settings)
        : panel{panel}
        , settings{settings}
        , gdbPathLabel{*panel, "GDB Path: "}
        , gdbPath{*panel, "gdb.exe"}
        , fullyReadSymbols{*panel, "Fully Read Symbols"}
        , neverReadSymbols{*panel, "Never Read Symbols"}
        , returnChildResult{*panel, "Return child result"}
        , initCommandFileCheck{*panel, "Init Command File: "}
        , initCommandFile{*panel, ""}
        , commandFileCheck{*panel, "Command File: "}
        , commandFile{*panel, ""}
        , ignoreHomeGdbInit{*panel, "Ignore ~/.gdbinit"}
        , ignoreAllGdbInit{*panel, "Ignore all .gdbinit"}
        , environmentOverwrites{*panel, "Environment overwrites Target Environment"}
        , changeEnvironment{*panel, "Change Environment"}
        , layout{*panel}
        , dirty{false}
    {
    }
//#####################################################################################################################
    GdbFrame::GdbFrame(nana::window parent, DebuggerSettings* settings)
        : nana::panel<true>{parent}
        , elements_{new GdbFrameImpl(this, settings)}
    {
        setLayout();
        enabled(false);
        setupEvents();
    }
//---------------------------------------------------------------------------------------------------------------------
    GdbFrame::~GdbFrame() = default;
//---------------------------------------------------------------------------------------------------------------------
    void GdbFrame::loadFromSettings()
    {
        auto runArgs = GdbSettings{};
        if (elements_->settings->runArguments)
            runArgs = elements_->settings->runArguments.value();

        elements_->initCommandFileCheck.check(runArgs.initCommandFile.has_value());
        elements_->initCommandFile.enabled(runArgs.initCommandFile.has_value());
        elements_->commandFileCheck.check(runArgs.commandFile.has_value());
        elements_->commandFile.enabled(runArgs.commandFile.has_value());

        elements_->gdbPath.caption(runArgs.gdbPath.value_or("gdb.exe"));
        elements_->fullyReadSymbols.check(runArgs.fullyReadSymbols.value_or(false));
        elements_->neverReadSymbols.check(runArgs.neverReadSymbols.value_or(false));
        elements_->returnChildResult.check(runArgs.returnChildResult.value_or(true));
        elements_->initCommandFile.caption(runArgs.initCommandFile.value_or(""));
        elements_->commandFile.caption(runArgs.commandFile.value_or(""));
        elements_->ignoreHomeGdbInit.check(runArgs.ignoreHomeGdbInit.value_or(false));
        elements_->ignoreAllGdbInit.check(runArgs.ignoreAllGdbInit.value_or(false));
    }
//---------------------------------------------------------------------------------------------------------------------
    void GdbFrame::enabled(bool enable)
    {
        elements_->gdbPathLabel.enabled(enable);
        elements_->gdbPath.enabled(enable);

        elements_->fullyReadSymbols.enabled(enable);
        elements_->neverReadSymbols.enabled(enable);
        elements_->returnChildResult.enabled(enable);

        elements_->initCommandFileCheck.enabled(enable);
        //elements_->initCommandFile.enabled(enable);

        elements_->commandFileCheck.enabled(enable);
        //elements_->commandFile.enabled(enable);

        elements_->ignoreHomeGdbInit.enabled(enable);
        elements_->ignoreAllGdbInit.enabled(enable);

        elements_->environmentOverwrites.enabled(enable);
        elements_->changeEnvironment.enabled(enable);
    }
//---------------------------------------------------------------------------------------------------------------------
    void GdbFrame::setLayout()
    {
        auto& layout = elements_->layout;

        layout.field("GdbPathLabel") << elements_->gdbPathLabel;
        layout.field("GdbPath") << elements_->gdbPath;

        layout.field("FullyReadSymbols") << elements_->fullyReadSymbols;
        layout.field("NeverReadSymbols") << elements_->neverReadSymbols;
        layout.field("ReturnChildResult") << elements_->returnChildResult;

        layout.field("InitCommandFileCheck") << elements_->initCommandFileCheck;
        layout.field("InitCommandFile") << elements_->initCommandFile;

        layout.field("CommandFileCheck") << elements_->commandFileCheck;
        layout.field("CommandFile") << elements_->commandFile;

        layout.field("IgnoreHomeGdbInit") << elements_->ignoreHomeGdbInit;
        layout.field("IngoreAllGdbInit") << elements_->ignoreAllGdbInit;

        layout.field("EnvironmentOverwrites") << elements_->environmentOverwrites;
        layout.field("ChangeEnvironment") << elements_->changeEnvironment;

        layout.div(layoutString);
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void GdbFrame::setupEvents()
    {
        elements_->changeEnvironment.events().click([this](auto const& event)
        {
            std::unordered_map <std::string, Environment> envs;
            if (elements_->settings->runArguments && elements_->settings->runArguments.value().environment)
                envs["Default"] = elements_->settings->runArguments.value().environment.value().environment;
            else
                envs["Default"];
            EnvironmentOptions envOpts{
                elements_->panel->parent(),
                &envs,
                [this, &envs]()
                {
                    if (!elements_->settings->runArguments)
                        elements_->settings->runArguments.emplace();
                    if (!elements_->settings->runArguments.value().environment)
                        elements_->settings->runArguments.value().environment.emplace();
                    elements_->settings->runArguments.value().environment.value().environment = envs["Default"];
                    elements_->settings->runArguments.value().environment.value().environmentIsAdditive =
                        !elements_->environmentOverwrites.checked()
                    ;
                    elements_->dirty = true;
                },
                true
            };
            envOpts.show();
        });

        #define INIT_RARGS if (!elements_->settings->runArguments) elements_->settings->runArguments.emplace();

        elements_->gdbPath.events().text_changed([this](auto const&) {
            INIT_RARGS;
            elements_->settings->runArguments.value().gdbPath = elements_->gdbPath.caption();
            elements_->dirty = true;
        });
        elements_->fullyReadSymbols.events().checked([this](auto const&) {
            INIT_RARGS;
            elements_->settings->runArguments.value().fullyReadSymbols = elements_->fullyReadSymbols.checked();
            elements_->dirty = true;
        });
        elements_->neverReadSymbols.events().checked([this](auto const&) {
            INIT_RARGS;
            elements_->settings->runArguments.value().neverReadSymbols = elements_->neverReadSymbols.checked();
            elements_->dirty = true;
        });
        elements_->returnChildResult.events().checked([this](auto const&) {
            INIT_RARGS;
            elements_->settings->runArguments.value().returnChildResult = elements_->returnChildResult.checked();
            elements_->dirty = true;
        });
        elements_->initCommandFileCheck.events().checked([this](auto const&) {
            INIT_RARGS;
            if (!elements_->initCommandFileCheck.checked())
                elements_->settings->runArguments.value().initCommandFile = std::nullopt;
            else
                elements_->settings->runArguments.value().initCommandFile = elements_->initCommandFile.caption();
            elements_->initCommandFile.enabled(elements_->initCommandFileCheck.checked());
            elements_->dirty = true;
        });
        elements_->initCommandFile.events().text_changed([this](auto const&) {
            INIT_RARGS;
            if (!elements_->initCommandFile.caption().empty())
                elements_->settings->runArguments.value().initCommandFile = elements_->initCommandFile.caption();
            elements_->dirty = true;
        });
        elements_->commandFileCheck.events().checked([this](auto const&) {
            INIT_RARGS;
            if (!elements_->commandFileCheck.checked())
                elements_->settings->runArguments.value().commandFile = std::nullopt;
            else
                elements_->settings->runArguments.value().commandFile = elements_->commandFile.caption();
            elements_->commandFile.enabled(elements_->commandFileCheck.checked());
            elements_->dirty = true;
        });
        elements_->commandFile.events().text_changed([this](auto const&) {
            INIT_RARGS;
            if (!elements_->commandFile.caption().empty())
                elements_->settings->runArguments.value().commandFile = elements_->commandFile.caption();
            elements_->dirty = true;
        });
        elements_->ignoreHomeGdbInit.events().checked([this](auto const&) {
            INIT_RARGS;
            elements_->settings->runArguments.value().ignoreHomeGdbInit = elements_->ignoreHomeGdbInit.checked();
            elements_->dirty = true;
        });
        elements_->ignoreAllGdbInit.events().checked([this](auto const&) {
            INIT_RARGS;
            elements_->settings->runArguments.value().ignoreAllGdbInit = elements_->ignoreAllGdbInit.checked();
            elements_->dirty = true;
        });
        elements_->environmentOverwrites.events().checked([this](auto const&) {
            INIT_RARGS;
            if (elements_->settings->runArguments.value().environment)
                elements_->settings->runArguments.value().environment.value().environmentIsAdditive = !elements_->environmentOverwrites.checked();
            elements_->dirty = true;
        });
        elements_->changeEnvironment;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool GdbFrame::isDirty() const
    {
        return elements_->dirty;
    }
//---------------------------------------------------------------------------------------------------------------------
    void GdbFrame::resetDirty()
    {
        elements_->dirty = false;
    }
//#####################################################################################################################
}
