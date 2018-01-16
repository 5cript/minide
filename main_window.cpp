#include "main_window.hpp"
#include "editor.hpp"
#include "log_tabs.hpp"
#include "project_tree_renderer.hpp"
#include "toolbar.hpp"

#include <nana/gui/msgbox.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/filebox.hpp>

#include <iostream>

using namespace std::string_literals;

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
        nana::toolbar toolbar;
        Editor editor;
        LogTabs logTabs;

        // Layout
        nana::place layout;

        // Other
        Theme theme;
        ProjectTreeRenderer treeRenderer;
        int lastMenuKeypress; // workaround, because
    };
//---------------------------------------------------------------------------------------------------------------------
    MainWindowImpl::MainWindowImpl()
        : form{nana::API::make_center(1000,600)}
        , menu{form}
        , projectTree{form}
        , toolbar{form}
        , editor{form}
        , logTabs{form}
        , layout{form}
        , theme{}
        , treeRenderer{projectTree.renderer()}
        , lastMenuKeypress{0}
    {
    }
//#####################################################################################################################
    MainWindow::MainWindow()
        : elements_{new MainWindowImpl}
        , settings_{}
        , environment_{}
        , workspace_{&settings_, &environment_}
    {
        setLayout();
        setupMenu();
        loadTheme({});
        registerTreeEvents();
        populateToolbar();
        setupToolbarEvents();
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::populateToolbar()
    {
        MinIDE::populateToolbar(elements_->toolbar);
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::setLayout()
    {
        auto& layout = elements_->layout;

        layout.field("editor") << elements_->editor;
        layout.field("logTabs") << elements_->logTabs;
        layout.field("projectBox") << elements_->projectTree;
        layout.field("toolbar") << elements_->toolbar;

        layout.div(layoutString);
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::setupMenu()
    {
        auto& menu = elements_->menu;
        menu.push_back("File");
        menu.at(0).append("Open Workspace", [this](nana::menu::item_proxy& ip)
        {
            nana::filebox fb(elements_->form, true);
            fb.add_filter("MinIDE Workspace", "*.midews");
            if (fb())
            {
                workspace_.loadWorkspace(fb.file());
                reloadProjectTree();
            }
        });
        menu.at(0).append("Open Project", [this](nana::menu::item_proxy& ip)
        {
            nana::filebox fb(elements_->form, true);
            fb.add_filter("CMakeLists", "CMakeLists.txt");
            if (fb())
            {
                workspace_.loadWorkspace();
                auto* project = workspace_.addProject(filesystem::path{fb.file()}.parent_path());
                reloadProjectTree();
                auto item = elements_->projectTree.find("workspace/"s + project->name());
                for (; !item.empty(); item = item.owner())
                    item.expand(true);
            }
        });

        // WORKAROUND - Alt Gr is not focusing the menu anymore
        menu.events().key_press([this](auto const& kb) {
            elements_->lastMenuKeypress = kb.key;
        });
        menu.events().focus([this](auto const& arg) {
            if (arg.getting && arg.focus_reason == nana::arg_focus::reason::general && elements_->lastMenuKeypress != 18)
                elements_->editor.focusTextbox();
        });
        // WORKAROUND END
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::loadTheme(Theme const& theme)
    {
        elements_->editor.loadTheme(theme);
        elements_->logTabs.loadTheme(theme);
        elements_->projectTree.bgcolor(theme.largeBackgrounds);
        elements_->projectTree.fgcolor(theme.textForegrounds);

        elements_->treeRenderer.imbueTheme(theme);
        elements_->projectTree.renderer(elements_->treeRenderer);
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::reloadProjectTree()
    {
        elements_->projectTree.insert("workspace", "Workspace").icon("resources/house.png");
        for (auto const& project : *workspace_.projects())
        {
            elements_->projectTree.insert("workspace/"s + project->name(), project->name());

            for (auto const& directory : *project->directories())
                elements_->projectTree.insert("workspace/"s + project->name() + "/" + directory.string(), directory.filename().string());

            for (auto const& file : *project->files())
                elements_->projectTree.insert("workspace/"s + project->name() + "/" + file.string(), file.filename().string());
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::registerTreeEvents()
    {
        elements_->projectTree.events().dbl_click([this](nana::arg_mouse const& arg)
        {
            if (!arg.is_left_button())
                return;

            auto item = elements_->projectTree.selected();
            if (item.empty())
                return;

            // reconstruct path:
            path p;
            auto i = item;
            for (; !i.empty() && !i.owner().empty() && i.owner().key() != "workspace"; i = i.owner())
                p = path{i.key()} / p;

            auto* project = workspace_.projectByName(i.key());
            if (project)
                elements_->editor.loadFile(project->rootDir(), p);
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::setupToolbarEvents()
    {
        elements_->toolbar.events().selected([this](auto const& event)
        {
            auto* activeProject = workspace_.activeProject();
            if (!activeProject)
                return;

            switch (static_cast <ToolbarElement> (event.button))
            {
                default: break;
                case ToolbarElement::CMake:
                case ToolbarElement::Build:
                case ToolbarElement::Run:
                    activeProject->setProcessOutputCallback([this](std::string const& str){
                        elements_->logTabs.addText(str);
                    });
                    elements_->logTabs.clear();
            }

            switch (static_cast <ToolbarElement> (event.button))
            {
                default: break;
                case ToolbarElement::Save:
                {
                    elements_->editor.save();
                    break;
                }
                case ToolbarElement::SaveAll:
                {
                    elements_->editor.saveAll();
                    break;
                }
                case ToolbarElement::CMake:
                {
                    activeProject->buildStep(0, true); // CMAKE
                    break;
                }
                case ToolbarElement::Build:
                {
                    activeProject->buildStep(1, true); // MAKE
                    break;
                }
                case ToolbarElement::Run:
                {
                    activeProject->run(true); // RUN
                    break;
                }
            }
        });
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
