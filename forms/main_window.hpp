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
            #include "layouts/MainWindow.layout"
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
        void refreshTargets();

    private:
        std::unique_ptr <MainWindowImpl> elements_;
    };
}
