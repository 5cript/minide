#include "workspace.hpp"

#include "cmake_project.hpp"

namespace MinIDE
{
//#####################################################################################################################
    Workspace::Workspace(Settings* settings)
        : settings_{settings}
        , workspaceRoot_{}
        , workspaceFile_{}
        , projects_{}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void Workspace::loadWorkspace()
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void Workspace::loadWorkspace(path const& workspaceFile)
    {
        workspaceRoot_ = workspaceFile.parent_path();
        workspaceFile_ = workspaceFile;
    }
//---------------------------------------------------------------------------------------------------------------------
    void Workspace::addProject(path const& relativeProjectDir)
    {
        auto projectDir = workspaceRoot_ / relativeProjectDir;
        if (filesystem::exists(projectDir / "CMakeLists.txt"))
        {
            projects_.push_back(std::make_unique <CMakeProject> (projectDir));
        }
    }
//#####################################################################################################################
}
