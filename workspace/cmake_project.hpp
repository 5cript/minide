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
        /**
         *  Creates a new CMake Project.
         *  @param settings The global settings relevant for all types of projects. (Toolboxes, Environments)
         *  @param rootDir The root dir of the projects which contains the CMakeLists.txt
         */
        CMakeProject(GlobalPersistence* settings, path const& rootDir);

        /**
         *  Load a project from a directory (which is equivallent of setting the root dir containing the cmake file).
         *  This function MIGHT parse the CMakeFile in the future.
         */
        void load(path const& rootDir) override;

        /**
         *  Do a build step. Build steps are different for different project types.
         *  @param step The step (0 = CMake, 1 = Build).
         */
        void buildStep(int step, std::string const& target) override;

        /**
         *  Run the output (depends on cmake file, might need parsing or setting).
         */
        void run(std::string const& target) override;

        /**
         *  Retrieve the project type as a string "cmake".
         */
        std::string type() const;

        /**
         *  Add a build target.
         */
        void addTarget(ProjectPersistence::CMakeBuildProfile const& target);
        ProjectPersistence::CMakeBuildProfile* getTarget(std::string const& target) override;

    private:
        void runExternal(std::string const& command, ProjectPersistence::CMakeBuildProfile* target);
        void runCMake(ProjectPersistence::CMakeBuildProfile* target);
        void runMake(ProjectPersistence::CMakeBuildProfile* target);

        /**
         *  Where shall the project be built in (often it is a build directory in the project root, but not necessarily)?
         */
        path buildDir(ProjectPersistence::CMakeBuildProfile* target) const;
        void loadEnvironment(ProjectPersistence::CMakeBuildProfile* target);

    private:
        std::unique_ptr <AsyncProcess> process_;
    };
}
