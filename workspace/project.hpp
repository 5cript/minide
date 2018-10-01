#pragma once

#include "../global_settings/global_persistence.hpp"
#include "../filesystem.hpp"

#include "project_file/build_profiles.hpp"

#include "project_events.hpp"
#include "../event_manager.hpp"

#include <memory>

namespace MinIDE
{
    struct ProjectImpl;

    class Project
    {
    public:
        using event_manager_type = EventManagement::EventManager<
            ProjectEvents,
            5,
            int, std::string
        >;

    public:
        Project(GlobalPersistence* settings);
        virtual ~Project();

        virtual void load(path const& rootDir);

        /**
         *  Different project types probably need a different amount of build steps.
         */
        virtual void buildStep(int step, std::string const& target) = 0;

        /**
         *  Run the build result.
         */
        virtual void run(std::string const& target) = 0;

        /**
         *  Run with debugger attached.
         */
        virtual void runDebug(std::string const& target) = 0;

        /**
         *  Kill whatever process is currently running.
         */
        virtual void killProcess() = 0;

        /**
         *  Return the type of the project as string.
         */
        virtual std::string type() const = 0;

        /**
         *  Returns the project root directory (everything belonging to the project is in here or deeper).
         */
        path rootDir() const;

        /**
         *  Returns the project name.
         */
        std::string name() const;

        /**
         *  Return all files associated with the project (recursively determined by glob expression).
         */
        std::vector <path> const* files() const;

        /**
         *  Return all directories associated with the project (recursive).
         */
        std::vector <path> const* directories() const;

        /**
         *  Callback for process output from the program being run.
         */
        //void setProcessOutputCallback(std::function <void(std::string const&)> const& cb);

        /**
         *  Returns all build target names (e.g. Debug, Release, ...).
         */
        std::vector <std::string> getBuildTargetNames() const;

        /**
         *  Remove a build target from the project.
         */
        void removeTarget(std::string const& targetName);

        /**
         *  Save project settings (writes project file to disk).
         */
        void saveSettings();

        /**
         *  Add a callback for a certain event.
         */
        template <typename FunctionType>
        void registerCallback(ProjectEvents type, FunctionType const& cb)
        {
            events_.add(type, cb);
        }

        /**
         *  Add a callback for a certain event and give it a name.
         */
        template <typename FunctionType>
        void registerCallback(ProjectEvents type, std::string const& name, FunctionType const& cb)
        {
            events_.add(type, name, cb);
        }

        /**
         *  Return a detailed build profile from target name. Must be cast to correct build profile.
         */
        virtual ProjectPersistence::BuildProfile* getTarget(std::string const& target);

    protected:
        void glob(
            path const& rootDir,
            std::vector <std::string> const& masks,
            std::vector <std::string> const& directoryBlackList
        );
        void loadProjectFiles();
        void saveProjectFiles();
        GlobalPersistence* getSettings();

    protected:
        std::unique_ptr <ProjectImpl> impl_;
        event_manager_type events_;
    };
}
