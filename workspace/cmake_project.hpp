#pragma once

#include "project.hpp"

#include "../global_settings/global_persistence.hpp"
#include "../async_process.hpp"
#include "../copyability.hpp"
#include "project_file/cmake_build_profile.hpp"

namespace MinIDE
{
    class CMakeProject : public Project
    {
    public:
        NONCOPY(CMakeProject);

    public:
        CMakeProject(GlobalPersistence* settings, path const& rootDir);

        void load(path const& rootDir) override;
        void buildStep(int step, std::string const& target) override;
        void run(std::string const& target) override;
        std::string type() const;
        void addTarget(ProjectPersistence::CMakeBuildProfile const& target);
        ProjectPersistence::CMakeBuildProfile* getTarget(std::string const& target) override;

    private:
        void runCMake(ProjectPersistence::CMakeBuildProfile* target);
        void runMake(ProjectPersistence::CMakeBuildProfile* target);
        path buildDir(ProjectPersistence::CMakeBuildProfile* target) const;
        void loadEnvironment(ProjectPersistence::CMakeBuildProfile* target);

    private:
        std::unique_ptr <AsyncProcess> process_;
    };
}
