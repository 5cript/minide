#pragma once

#include "../global_settings/global_persistence.hpp"
#include "../theme.hpp"

#include <memory>

namespace MinIDE
{
    struct MainWindowImpl;

    class MainWindow
    {
    public:
        constexpr static const char* layoutString =
            "<vertical <weight=25><toolbar weight=25> <<weight=15% projectBox>|<vertical <weight=70% editor>|<logTabs>>>"
        ;

    public:
        MainWindow(GlobalPersistence* globalSettings);
        ~MainWindow();

        void show();

        void loadTheme(Theme const& theme);

    private:
        void setLayout();
        void setupMenu();
        void reloadProjectTree();
        void registerTreeEvents();
        void populateToolbar();
        void setupToolbarEvents();

    private:
        std::unique_ptr <MainWindowImpl> elements_;
    };
}