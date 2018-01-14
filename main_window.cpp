#include "main_window.hpp"
#include "editor.hpp"
#include "log_tabs.hpp"

#include <nana/gui/msgbox.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/filebox.hpp>

#include <iostream>

namespace MinIDE
{
//#####################################################################################################################
    struct MainWindowImpl
    {
        MainWindowImpl();

        // Form
        nana::form form;

        // Widgets
        nana::menubar menu;
        nana::treebox projectTree;
        Editor editor;
        LogTabs logTabs;

        // Layout
        nana::place layout;

        // Other
        Theme theme;
    };
//---------------------------------------------------------------------------------------------------------------------
    MainWindowImpl::MainWindowImpl()
        : form{nana::API::make_center(1000,600)}
        , menu{form}
        , projectTree{form}
        , editor{form}
        , logTabs{form}
        , layout{form}
        , theme{}
    {
    }
//#####################################################################################################################
    MainWindow::MainWindow()
        : elements_{new MainWindowImpl}
    {
        setLayout();
        setupMenu();
        loadTheme({});
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::setLayout()
    {
        auto& layout = elements_->layout;

        layout.field("editor") << elements_->editor;
        layout.field("logTabs") << elements_->logTabs;
        layout.field("projectBox") << elements_->projectTree;

        layout.div(layoutString);
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::setupMenu()
    {
        auto& menu = elements_->menu;
        menu.push_back("&File");
        menu.at(0).append("Open Workspace", [this](nana::menu::item_proxy& ip)
        {
            nana::filebox fb(elements_->form, true);
            fb.add_filter("MinIDE Workspace", "*.midews");
            if (fb())
            {
                workspace_.loadWorkspace(fb.file());
            }
        });
        menu.at(0).append("Open Project", [this](nana::menu::item_proxy& ip)
        {
            nana::filebox fb(elements_->form, true);
            fb.add_filter("CMakeLists", "CMakeLists.txt");
            if (fb())
            {
                workspace_.loadWorkspace();
                workspace_.addProject(filesystem::path{fb.file()}.parent_path());
            }
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::loadTheme(Theme const& theme)
    {
        elements_->editor.loadTheme(theme);
        elements_->logTabs.loadTheme(theme);
        elements_->projectTree.bgcolor(theme.largeBackgrounds);
        elements_->projectTree.fgcolor(theme.textForegrounds);
    }
//---------------------------------------------------------------------------------------------------------------------
    MainWindow::~MainWindow() = default;
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::show()
    {
        elements_->form.show();
    }
//#####################################################################################################################
}
