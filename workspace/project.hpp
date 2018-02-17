#pragma once

#include "../global_settings/global_persistence.hpp"
#include "../filesystem.hpp"
#include "project_file/build_profiles.hpp"

#include <memory>

namespace MinIDE
{
    struct ProjectImpl;

    class Project
    {
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
         *  Return the type of the project as string.
         */
        virtual std::string type() const = 0;

        path rootDir() const;
        std::string name() const;
        std::vector <path> const* files() const;
        std::vector <path> const* directories() const;
        void setProcessOutputCallback(std::function <void(std::string const&)> const& cb);
        std::vector <std::string> getBuildTargetNames() const;
        void removeTarget(std::string const& targetName);

        void saveSettings();

        virtual ProjectPersistence::BuildProfile* getTarget(std::string const& target);

    protected:
        void glob(
            path const& rootDir,
            std::vector <std::string> const& masks,
            std::vector <std::string> const& directoryBlackList
        );
        void loadProjectFiles();
        void saveProjectFiles();

    protected:
        std::unique_ptr <ProjectImpl> impl_;
    };
}
