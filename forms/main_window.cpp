#include "main_window.hpp"
#include "main_window/editor.hpp"
#include "main_window/log_tabs.hpp"
#include "main_window/project_tree_renderer.hpp"
#include "main_window/toolbar.hpp"
#include "../gui_utility/show_message.hpp"

// Project
#include "../workspace/cmake_project.hpp"

// Persistence Related
#include "../workspace/workspace.hpp"
#include "../global_settings/build_base_settings.hpp"
#include "../workspace/project_file/cmake_build_profile.hpp"
#include "../resources.hpp"

// Other GUIs
#include "environment_options.hpp"
#include "tool_options.hpp"
#include "cmake_target_creator.hpp"
#include "debugger_options.hpp"
#include "creator.hpp"

// Widgets
#include <nana/gui/msgbox.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/filebox.hpp>

#include <iostream>

using namespace std::string_literals;

namespace MinIDE
{
//#####################################################################################################################
    enum MenuEntries : std::size_t
    {
        File,
        Project,
        Settings
    };
//#####################################################################################################################
    struct MainWindowImpl
    {
        MainWindowImpl(GlobalPersistence* persistence);

        // Form
        nana::form form;

        // Widgets
        nana::menubar menu;
        nana::treebox projectTree;
        nana::toolbar toolbar;
        Editor editor;
        LogTabs logTabs;
        nana::combox activeProject;
        nana::combox targetSelector;

        // Layout
        nana::place layout;

        // Other
        Theme theme;
        ProjectTreeRenderer treeRenderer;
        int lastMenuKeypress; // workaround, because

        // Settings
        GlobalPersistence* persistence;
        Workspace workspace;
    };
//---------------------------------------------------------------------------------------------------------------------
    MainWindowImpl::MainWindowImpl(GlobalPersistence* persistence)
        : form{nana::API::make_center(1000,600)}
        , menu{form}
        , projectTree{form}
        , toolbar{form}
        , editor{form}
        , logTabs{form}
        , activeProject{form}
        , targetSelector{form}
        , layout{form}
        , theme{}
        , treeRenderer{projectTree.renderer()}
        , lastMenuKeypress{0}
        , persistence{persistence}
        , workspace{persistence}
    {
    }
//#####################################################################################################################
    MainWindow::MainWindow(GlobalPersistence* globalSettings)
        : elements_{new MainWindowImpl(globalSettings)}
    {
        setLayout();
        setupMenu();
        loadTheme({});
        registerTreeEvents();
        populateToolbar();
        setupToolbarEvents();

        elements_->editor.sidepanelClickEvent.connect(
            [this](path const& file, int line, nana::arg_mouse keyState){
                auto* activeProject = elements_->workspace.activeProject();
                if (!activeProject)
                    return;
                auto* cmakeProject = dynamic_cast <CMakeProject*> (activeProject);
                if (!cmakeProject)
                    return;

                if (keyState.left_button)
                    cmakeProject->debugger().toggleBreakpoint(file, line);
            }
        );
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::populateToolbar()
    {
        MinIDE::populateToolbar(elements_->toolbar);
    }
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::setLayout()
    {
        auto& layout = elements_->layout;

        layout.field("Editor") << elements_->editor;
        layout.field("LogTabs") << elements_->logTabs;
        layout.field("ProjectBox") << elements_->projectTree;
        layout.field("Toolbar") << elements_->toolbar;
        layout.field("TargetSelector") << elements_->targetSelector;
        layout.field("ProjectSelector") << elements_->activeProject;

        layout.div(loadResource("layouts/main_window.layout"));
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::refreshTargets()
    {
        /*
        elements_->environmentSelector.clear();
        for (auto const& i : elements_->persistence->environments)
            elements_->environmentSelector.push_back(i.first);
        if (elements_->environmentSelector.the_number_of_options() > 0)
            elements_->environmentSelector.option(0);
        */
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::openProject(path const& directory)
    {
        elements_->workspace.loadWorkspace();
        auto* project = elements_->workspace.addProject(directory);
        if (project == nullptr)
            return;
        reloadProjectTree();
        auto item = elements_->projectTree.find("workspace/"s + project->name());
        for (; !item.empty(); item = item.owner())
            item.expand(true);
        refreshProjectSelector();
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::setupMenu()
    {
        auto& menu = elements_->menu;

        menu.push_back("File");
        menu.push_back("Project");
        menu.push_back("Settings");


        menu.at(File).append("Open Workspace", [this](nana::menu::item_proxy& ip)
        {
            nana::filebox fb(elements_->form, true);
            fb.add_filter("MinIDE Workspace", "*.midews");
            auto files = fb.show();
            if (!files.empty())
            {
                auto file = files.front();
                elements_->workspace.loadWorkspace(file.string());
                reloadProjectTree();
                refreshProjectSelector();
            }
        });
        menu.at(File).append("Open Project", [this](nana::menu::item_proxy& ip)
        {
            nana::filebox fb(elements_->form, true);
            fb.add_filter("CMakeLists", "CMakeLists.txt");
            auto files = fb.show();
            if (!files.empty())
            {
                auto file = files.front();
                openProject(filesystem::path{file.string()}.parent_path());
            }
        });
        menu.at(File).append("New Project", [this](auto& item)
        {
            Creator creator(elements_->form, loadResource("wizards/projects.json"));

            auto selected = creator.show();
            if (!selected)
                return;
            else
            {
                try
                {
                    std::cout << selected.value().name();
                    auto path = selected.value().startWizard();
                    if (path)
                        openProject(path.value());
                }
                catch (jailbreak_error const& exc)
                {
                    showError(exc.what());
                }
                catch (std::exception const& exc)
                {
                    showError(exc.what());
                }
            }
        });

        // Project
        menu.at(Project).append("Add Build Target", [this](auto& item)
        {
            addTarget();
        });
        menu.at(Project).enabled(0, false);
        menu.at(Project).append("Edit Current Target", [this](auto& item)
        {
            editTarget();
        });
        menu.at(Project).enabled(1, false);
        menu.at(Project).append("Remove Build Target", [this](auto& item)
        {
            removeTarget();
        });
        menu.at(Project).enabled(2, false);

        // Settings
        menu.at(Settings).append("Environment Settings", [this](auto& item)
        {
            EnvironmentOptions envOpts{
                elements_->form,
                &elements_->persistence->environments,
                [this]()
                {
                    elements_->persistence->save();
                }
            };
            envOpts.show();
        });

        menu.at(Settings).append("Tool Settings", [this](auto& item)
        {
            ToolOptions toolOpts{elements_->form, elements_->persistence};
            toolOpts.show();
        });

        menu.at(Settings).append("Debugger Settings", [this](auto& item)
        {
            DebuggerOptions debuggerOpts{
                elements_->form,
                &elements_->persistence->debugger,
                [this]()
                {
                    elements_->persistence->save();
                }
            };
            debuggerOpts.show();
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
    void MainWindow::addTarget()
    {
        auto* project = elements_->workspace.projectByName(elements_->activeProject.caption());
        if (!project)
            return;

        if (project->type() == "cmake")
        {
            CMakeTargetCreator targetCreator{elements_->form, elements_->persistence};
            targetCreator.setup(
                [this](){
                    std::vector <std::string> envs;
                    for (auto const& env : elements_->persistence->environments)
                        envs.push_back(env.first);
                    return envs;
                }(),
                [this](){
                    std::vector <std::string> tools;
                    for (auto const& tool : elements_->persistence->tooling)
                        tools.push_back(tool.first);
                    return tools;
                }()
            );
            targetCreator.show();
            if (targetCreator.clickedSave())
            {
                static_cast <CMakeProject*> (project)->addTarget(
                    targetCreator.getTarget()
                );
                elements_->targetSelector.push_back(targetCreator.name());
            }
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::editTarget()
    {
        auto* project = elements_->workspace.projectByName(elements_->activeProject.caption());
        if (!project)
            return;

        if (project->type() == "cmake")
        {
            if (elements_->targetSelector.caption().empty())
                return;

            auto* target = static_cast <CMakeProject*> (project)->getTarget(elements_->targetSelector.caption());
            CMakeTargetCreator targetCreator{elements_->form, elements_->persistence};
            targetCreator.setup(
                [this](){
                    std::vector <std::string> envs;
                    for (auto const& env : elements_->persistence->environments)
                        envs.push_back(env.first);
                    return envs;
                }(),
                [this](){
                    std::vector <std::string> tools;
                    for (auto const& tool : elements_->persistence->tooling)
                        tools.push_back(tool.first);
                    return tools;
                }(),
                target
            );
            targetCreator.show();
            if (targetCreator.clickedSave())
            {
                *target = targetCreator.getTarget();
                if (targetCreator.originalName() != targetCreator.name())
                {
                    elements_->targetSelector.erase(elements_->targetSelector.option());
                    elements_->targetSelector.push_back(targetCreator.name());
                    elements_->targetSelector.option(elements_->targetSelector.the_number_of_options() - 1);
                }
                project->saveSettings();
            }
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::removeTarget()
    {
        auto* project = elements_->workspace.projectByName(elements_->activeProject.caption());
        if (!project)
            return;

        if (elements_->targetSelector.option() != nana::npos)
        {
            project->removeTarget(elements_->targetSelector.caption());
            elements_->targetSelector.erase(elements_->targetSelector.option());
            project->saveSettings();

            if (!elements_->targetSelector.empty())
                elements_->targetSelector.option(0);
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::refreshProjectSelector()
    {
        elements_->activeProject.clear();
        for (auto const& i : *elements_->workspace.projects())
            elements_->activeProject.push_back(i->name());

        if (elements_->activeProject.the_number_of_options() > 0)
        {
            elements_->activeProject.option(0);
            elements_->menu.at(Project).enabled(0, true);
            elements_->menu.at(Project).enabled(1, true);
            elements_->menu.at(Project).enabled(2, true);
        }
        auto& toolbar = elements_->toolbar;
        activate <ToolbarElement::CMake> (toolbar);
        activate <ToolbarElement::Build> (toolbar);
        activate <ToolbarElement::Run> (toolbar);
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::setActiveProject(std::string const& name)
    {
        auto* activeProject = elements_->workspace.projectByName(name);
        elements_->targetSelector.clear();
        for (auto&& target : activeProject->getBuildTargetNames())
            elements_->targetSelector.push_back(target);

        if (!elements_->targetSelector.empty())
            elements_->targetSelector.option(0);
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
        elements_->projectTree.insert("workspace", "Workspace").icon("resources/images/house.png");
        for (auto const& project : *elements_->workspace.projects())
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

            auto* project = elements_->workspace.projectByName(i.key());
            if (project)
                elements_->editor.loadFile(project->rootDir(), p);
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void MainWindow::setupToolbarEvents()
    {
        elements_->toolbar.events().selected([this](auto const& event)
        {
            auto* activeProject = elements_->workspace.activeProject();
            if (!activeProject)
                return;

            auto deactivateButtons = [&]()
            {
                deactivate <ToolbarElement::Build>(elements_->toolbar);
                deactivate <ToolbarElement::BuildAndRun>(elements_->toolbar);
                deactivate <ToolbarElement::Run>(elements_->toolbar);
                deactivate <ToolbarElement::CMake>(elements_->toolbar);
                activate <ToolbarElement::Cancel>(elements_->toolbar);
            };

            auto activateButtons = [&]()
            {
                activate <ToolbarElement::Build>(elements_->toolbar);
                activate <ToolbarElement::BuildAndRun>(elements_->toolbar);
                activate <ToolbarElement::Run>(elements_->toolbar);
                activate <ToolbarElement::CMake>(elements_->toolbar);
                deactivate <ToolbarElement::Cancel>(elements_->toolbar);
            };

            switch (static_cast <ToolbarElement> (event.button))
            {
                default: break;
                case ToolbarElement::CMake:
                case ToolbarElement::Build:
                case ToolbarElement::Run:
                case ToolbarElement::DebugContinue:
                    activeProject->registerCallback(ProjectEvents::Info, "info",
                        [this](std::string const& str) {
                            elements_->logTabs.addInfo(str);
                        }
                    );
                    activeProject->registerCallback(ProjectEvents::ProcessOutput, "build_log",
                        [this](std::string const& str) {
                            elements_->logTabs.addBuildLog(str);
                        }
                    );
                    activeProject->registerCallback(ProjectEvents::ProcessOutput, "process_output",
                        [this](std::string const& str) {
                            elements_->logTabs.addProgramOutput(str);
                        }
                    );
                    activeProject->registerCallback(ProjectEvents::ProcessOutput, "debugger_output",
                        [this](std::string const& str) {
                            elements_->logTabs.addDebuggerOutput(str);
                        }
                    );
                    activeProject->registerCallback(ProjectEvents::ProcessExited, "process_exit",
                        [activateButtons](int const&) {
                            activateButtons();
                        }
                    );
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
                    elements_->logTabs.clearBuildMessages();
                    elements_->logTabs.clearBuildLog();
                    elements_->logTabs.selectBuildLog();
                    if (promptTargetMissing())
                    {
                        activeProject->buildStep(0, elements_->targetSelector.caption()); // CMAKE
                        deactivateButtons();
                    }
                    break;
                }
                case ToolbarElement::Build:
                {
                    elements_->logTabs.clearBuildMessages();
                    elements_->logTabs.clearBuildLog();
                    elements_->logTabs.selectBuildLog();
                    if (promptTargetMissing())
                    {
                        activeProject->buildStep(1, elements_->targetSelector.caption()); // MAKE
                        deactivateButtons();
                    }
                    break;
                }
                case ToolbarElement::Run:
                {
                    elements_->logTabs.clearProgramOutput();
                    elements_->logTabs.selectProgramOutput();
                    if (promptTargetMissing())
                    {
                        activeProject->run(elements_->targetSelector.caption()); // RUN
                        deactivateButtons();
                    }
                    break;
                }
                case ToolbarElement::Cancel:
                {
                    activeProject->killProcess();
                    break;
                }
                case ToolbarElement::DebugContinue:
                {
                    elements_->logTabs.clearDebuggerOutput();
                    elements_->logTabs.selectDebuggerOutput();
                    if (promptTargetMissing())
                    {
                        auto cmakeProject = dynamic_cast <CMakeProject*>(activeProject);
                        if (cmakeProject)
                        {
                            if (!cmakeProject->isDebugging())
                                cmakeProject->runDebug(elements_->targetSelector.caption(), "Default"); // RUN DEBUG
                            else
                                cmakeProject->debugger().step();
                            deactivateButtons();
                        }
                    }
                    break;
                }
            }
        });

        elements_->targetSelector.events().selected([this](auto const& event)
        {
            if (elements_->targetSelector.option() != nana::npos)
            {
                auto* project = elements_->workspace.projectByName(elements_->activeProject.caption());
                if (!project)
                    return;
                if (project->type() != "cmake")
                    return;
                auto* target = static_cast <CMakeProject*> (project)->getTarget(elements_->targetSelector.caption());
                if (target && target->isDebugable)
                    activate <ToolbarElement::DebugContinue> (elements_->toolbar);
                else if (target)
                    deactivate <ToolbarElement::DebugContinue> (elements_->toolbar);
            }
        });

        elements_->activeProject.events().selected([this](auto const& event)
        {
            setActiveProject(event.widget.caption());
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    bool MainWindow::promptTargetMissing()
    {
        if (elements_->targetSelector.option() == nana::npos)
        {
            nana::msgbox box(elements_->form, "Select Target", nana::msgbox::ok);
            box.icon(nana::msgbox::icon_error) << "You must select a build target first, maybe create one.";
            box.show();
            return false;
        }
        return true;
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
