#pragma once

#include "project.hpp"

#include "../copyability.hpp"

namespace MinIDE
{
    class CMakeProject : public Project
    {
    public:
        NONCOPY(CMakeProject);

    public:
        CMakeProject() = default;
        CMakeProject(path const& rootDir);

        void loadEnvironment(path const& rootDir) override;

    private:

    };
}
