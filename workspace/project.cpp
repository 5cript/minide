#include "project.hpp"

#include "globber.hpp"

namespace MinIDE
{
//#####################################################################################################################
    Project::Project(GlobalPersistence* settings, std::string* currentEnvironment)
        : settings_{settings}
        , currentEnvironment_{currentEnvironment}
        , files_{}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    void Project::glob(
        path const& rootDir,
        std::vector <std::string> const& masks,
        std::vector <std::string> const& directoryBlackList
    )
    {
        Globber globber{rootDir.string()};
        globber.setDirectoryBlackList(directoryBlackList);
        files_ = globber.globRecursive(masks, false);

        Globber dirGlob{rootDir.string(), true};
        dirGlob.setDirectoryBlackList(directoryBlackList);
        directories_ = dirGlob.globRecursive(std::vector <std::string>{}, false);
    }
//---------------------------------------------------------------------------------------------------------------------
    path Project::rootDir() const
    {
        return rootDir_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Project::name() const
    {
        return rootDir_.filename().string();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <path> const* Project::files() const
    {
        return &files_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <path> const* Project::directories() const
    {
        return &directories_;
    }
//---------------------------------------------------------------------------------------------------------------------
    void Project::setProcessOutputCallback(std::function <void(std::string const&)> const& cb)
    {
        cb_ = cb;
    }
//#####################################################################################################################
}
