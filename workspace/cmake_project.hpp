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
        CMakeProject(GlobalPersistence* settings, std::string* currentEnvironment, path const& rootDir);

        void load(path const& rootDir) override;
        void buildStep(int step, bool debug) override;
        void run(bool debug) override;

    private:
        void runCMake(bool debug);
        void runMake(bool debug);
        path buildDir(bool debug) const;
        std::unique_ptr <AsyncProcess> process_;
    };
}
