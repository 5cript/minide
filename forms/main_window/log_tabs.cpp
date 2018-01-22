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
        nana::textbox log;
        nana::place layout;

        LogTabsImpl(nana::panel<true>& panel)
            : tabs{panel, true}
            , log{panel}
            , layout{panel}
        {
            //log.editable(false);

            tabs.push_back("Build log");
            tabs.push_back("Build messages");
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
    void LogTabs::addText(std::string const& data)
    {
        elements_->log.caption(elements_->log.caption() + data);
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::clear()
    {
        elements_->log.caption("");
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::setLayout()
    {
        elements_->layout.div(layoutString);
        elements_->layout["tabbar"] << elements_->tabs;
        elements_->layout["log"] << elements_->log;
        elements_->layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void LogTabs::loadTheme(Theme const& theme)
    {
        elements_->log.bgcolor(theme.largeBackgrounds);
        elements_->log.fgcolor(theme.textForegrounds);
    }
//---------------------------------------------------------------------------------------------------------------------
    LogTabs::~LogTabs() = default;
//#####################################################################################################################
}
