#pragma once

#include "../global_settings/global_persistence.hpp"
#include "../theme.hpp"

#include "../filesystem.hpp"

#include <memory>

namespace MinIDE
{
    struct MainWindowImpl;

    class MainWindow
    {
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
        void refreshProjectSelector();
        void addTarget();
        void editTarget();
        void removeTarget();

    private: // Project & Workspace Related
        void setActiveProject(std::string const& name);
        void openProject(path const& directory);

        /**
         *  @return Continue?
         */
        bool promptTargetMissing();

    private:
        std::unique_ptr <MainWindowImpl> elements_;
    };
}
