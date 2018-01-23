#pragma once

#include "project.hpp"

#include "../global_settings/global_persistence.hpp"
#include "../async_process.hpp"
#include "../copyability.hpp"

namespace MinIDE
{
    class CMakeProject : public Project
    {
    public:
        NONCOPY(CMakeProject);

    public:
        CMakeProject(GlobalPersistence* settings, path const& rootDir);

        void load(path const& rootDir) override;
        void buildStep(int step) override;
        void run() override;

    private:
        void runCMake();
        void runMake();
        path buildDir() const;
        std::unique_ptr <AsyncProcess> process_;
    };
}
