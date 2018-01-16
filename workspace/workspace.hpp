#pragma once

#include "../settings/settings.hpp"
#include "project.hpp"
#include "../filesystem.hpp"

#include <vector>
#include <memory>
#include <boost/optional.hpp>

namespace MinIDE
{
    class Workspace
    {
    public:
        Workspace(Settings* settings, Environment* environment);
        ~Workspace() = default;

        /**
         *  Initialize a new virtual workspace.
         */
        void loadWorkspace();

        /**
         *  Load a workspace.
         */
        void loadWorkspace(path const& workspaceFile);

        /**
         *  Add a project to the workspace.
         */
        Project* addProject(path const& relativeProjectDir);

        /**
         *  Get the active project.
         */
        Project* activeProject();

        /**
         *  Retrieve projects.
         */
        std::vector <std::unique_ptr <Project>>* projects();

        /**
         *  Retrieve a project by name or nullptr;
         */
        Project* projectByName(std::string const& name);

    private:
        Settings* settings_;
        Environment* environment_;
        path workspaceRoot_;
        boost::optional <path> workspaceFile_;
        std::vector <std::unique_ptr <Project>> projects_;
    };
}
