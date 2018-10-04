#pragma once

#include "../../copyability.hpp"
#include "../../theme.hpp"

#include <nana/gui/widgets/panel.hpp>
#include <memory>

namespace MinIDE
{
    struct LogTabsImpl;

    class LogTabs : public nana::panel<true>
    {
    public:
        constexpr static const char* layoutString =
            "<vertical margin=0 <tabbar max=18><log>>"
        ;

        NONCOPY(LogTabs);

    public:
        LogTabs(nana::window parent);
        ~LogTabs();

        void loadTheme(Theme const& theme);
        void addInfo(std::string const& data);
        void addBuildLog(std::string const& data);
        void addBuildMessage(std::string const& data);
        void addProgramOutput(std::string const& data);
        void addDebuggerOutput(std::string const& data);
        void clearAll();
        void clearInfo();
        void clearBuildLog();
        void clearBuildMessages();
        void clearProgramOutput();
        void clearDebuggerOutput();

        void selectBuildMessages();
        void selectBuildLog();
        void selectProgramOutput();
        void selectDebuggerOutput();

    private:
        void setLayout();

    private:
        std::unique_ptr <LogTabsImpl> elements_;
    };
}
