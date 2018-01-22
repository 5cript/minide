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
        void addText(std::string const& data);
        void clear();

    private:
        void setLayout();

    private:
        std::unique_ptr <LogTabsImpl> elements_;
    };
}
