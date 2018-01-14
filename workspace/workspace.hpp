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
        Workspace(Settings* settings);
        ~Workspace() = default;

        void loadWorkspace();
        void loadWorkspace(path const& workspaceFile);

        void addProject(path const& relativeProjectDir);

    private:
        Settings* settings_;
        path workspaceRoot_;
        boost::optional <path> workspaceFile_;
        std::vector <std::unique_ptr <Project>> projects_;
    };
}
