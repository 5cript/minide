#pragma once

#include "../settings/settings.hpp"
#include "project.hpp"

#include "../copyability.hpp"

namespace MinIDE
{
    class CMakeProject : public Project
    {
    public:
        NONCOPY(CMakeProject);

    public:
        CMakeProject(Settings* settings, path const& rootDir);

        void loadEnvironment(path const& rootDir) override;

    private:

    };
}
