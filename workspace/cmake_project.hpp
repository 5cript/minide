#pragma once

#include "project.hpp"

#include "../settings/settings.hpp"
#include "../async_process.hpp"
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
        void buildStep(int step, bool debug) override;

    private:
        void runCMake(bool debug);
        std::unique_ptr <AsyncProcess> process_;
    };
}
