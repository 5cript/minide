#include "log_tabs.hpp"

#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>

namespace MinIDE
{
//#####################################################################################################################
    struct LogTabsImpl
    {
        nana::tabbar <std::string> tabs;
        nana::textbox info;
        nana::textbox buildLog;
        nana::textbox buildMessages;
        nana::textbox processOutput;
        nana::textbox debuggerOutput;
        nana::place layout;

        LogTabsImpl(nana::panel<true>& panel)
            : tabs{panel, true}
            , info{panel}
            , buildLog{panel}
            , buildMessages{panel}
            , processOutput{panel}
            , debuggerOutput{panel}
            , layout{panel}
        {
            tabs.append("Info", info);
            tabs.append("Build Log", buildLog);
            tabs.append("Build Messages", buildMessages);
            tabs.append("Process Output", processOutput);
            tabs.append("Debugger Output", debuggerOutput);

            info.editable(false);
            buildLog.editable(false);
            buildMessages.editable(false);
            processOutput.editable(false);
            debuggerOutput.editable(false);
        }
    };
//#####################################################################################################################
    LogTabs::LogTabs(nana::window parent)
        : nana::panel<true>{parent}
        , elements_{new LogTabsImpl(*this)}
    {
        borderless(true);

        setLayout();
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::addInfo(std::string const& data)
    {
        elements_->info.caption(elements_->info.caption() + data);
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::addBuildLog(std::string const& data)
    {
        elements_->buildLog.caption(elements_->buildLog.caption() + data);
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::addBuildMessage(std::string const& data)
    {
        elements_->buildMessages.caption(elements_->buildMessages.caption() + data);
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::addProgramOutput(std::string const& data)
    {
        elements_->processOutput.caption(elements_->processOutput.caption() + data);
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::addDebuggerOutput(std::string const& data)
    {
        elements_->debuggerOutput.caption(elements_->debuggerOutput.caption() + data);
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::clearAll()
    {
        elements_->info.caption("");
        elements_->buildLog.caption("");
        elements_->buildMessages.caption("");
        elements_->processOutput.caption("");
        elements_->debuggerOutput.caption("");
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::clearInfo()
    {
        elements_->info.caption("");
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::clearBuildLog()
    {
        elements_->buildLog.caption("");
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::clearBuildMessages()
    {
        elements_->buildMessages.caption("");
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::clearProgramOutput()
    {
        elements_->processOutput.caption("");
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::clearDebuggerOutput()
    {
        elements_->debuggerOutput.caption("");
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::selectBuildMessages()
    {
        elements_->tabs.activated(2);
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::selectBuildLog()
    {
        elements_->tabs.activated(1);
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::selectProgramOutput()
    {
        elements_->tabs.activated(3);
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::selectDebuggerOutput()
    {
        elements_->tabs.activated(4);
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::setLayout()
    {
        elements_->layout.div(layoutString);
        elements_->layout["tabbar"] << elements_->tabs;
        elements_->layout["log"]
            .fasten(elements_->info)
            .fasten(elements_->buildLog)
            .fasten(elements_->buildMessages)
            .fasten(elements_->processOutput)
            .fasten(elements_->debuggerOutput)
        ;
        elements_->layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::loadTheme(Theme const& theme)
    {
        auto setEditorColoring = [&theme](auto& editor)
        {
            editor.bgcolor(theme.largeBackgrounds);
            editor.fgcolor(theme.textForegrounds);
        };

        setEditorColoring(elements_->info);
        setEditorColoring(elements_->buildLog);
        setEditorColoring(elements_->buildMessages);
        setEditorColoring(elements_->processOutput);
        setEditorColoring(elements_->debuggerOutput);
    }
//---------------------------------------------------------------------------------------------------------------------
    LogTabs::~LogTabs() = default;
//#####################################################################################################################
}
