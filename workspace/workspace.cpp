#include "workspace.hpp"

#include "cmake_project.hpp"

namespace MinIDE
{
//#####################################################################################################################
    Workspace::Workspace(GlobalPersistence* settings, std::string* currentEnvironment)
        : settings_{settings}
        , currentEnvironment_{currentEnvironment}
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
    std::vector <std::unique_ptr <Project>>* Workspace::projects()
    {
        return &projects_;
    }
//---------------------------------------------------------------------------------------------------------------------
    Project* Workspace::addProject(path const& relativeProjectDir)
    {
        auto projectDir = workspaceRoot_ / relativeProjectDir;
        if (filesystem::exists(projectDir / "CMakeLists.txt"))
        {
            //projects_.push_back(std::make_unique <CMakeProject> (settings_, environment_, projectDir));
            //return projects_.back().get();
        }
        return nullptr;
    }
//---------------------------------------------------------------------------------------------------------------------
    Project* Workspace::projectByName(std::string const& name)
    {
        for (auto const& project : projects_)
            if (project && project->name() == name)
                return project.get();

        return nullptr;
    }
//---------------------------------------------------------------------------------------------------------------------
    Project* Workspace::activeProject()
    {
        if (projects_.empty())
            return nullptr;
        else
            return projects_[0].get();
    }
//#####################################################################################################################
}
