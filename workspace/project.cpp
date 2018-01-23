#include "project.hpp"
#include "project_impl.hpp"

#include "globber.hpp"

namespace MinIDE
{
//#####################################################################################################################
    Project::Project(GlobalPersistence* settings)
        : impl_{new ProjectImpl(settings)}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    Project::~Project() = default;
//---------------------------------------------------------------------------------------------------------------------
    void Project::saveSettings()
    {
        saveProjectFiles();
    }
//---------------------------------------------------------------------------------------------------------------------
    void Project::loadProjectFiles()
    {
        auto projectFile = impl_->rootDir / ".minproj";
        auto localFile = impl_->rootDir / ".minproj.local";

        if (filesystem::exists(projectFile))
            impl_->projectFile.load(projectFile);
        if (filesystem::exists(projectFile))
            impl_->projectFile.load(localFile);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Project::saveProjectFiles()
    {
        auto projectFile = impl_->rootDir / ".minproj";
        auto localFile = impl_->rootDir / ".minproj.local";

        impl_->projectFile.save(projectFile);
        impl_->projectFile.save(localFile);
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
        impl_->files = globber.globRecursive(masks, false);

        Globber dirGlob{rootDir.string(), true};
        dirGlob.setDirectoryBlackList(directoryBlackList);
        impl_->directories = dirGlob.globRecursive(std::vector <std::string>{}, false);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Project::load(path const& rootDir)
    {
        impl_->rootDir = rootDir;
        loadProjectFiles();
    }
//---------------------------------------------------------------------------------------------------------------------
    path Project::rootDir() const
    {
        return impl_->rootDir;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Project::name() const
    {
        return impl_->rootDir.filename().string();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <path> const* Project::files() const
    {
        return &impl_->files;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <path> const* Project::directories() const
    {
        return &impl_->directories;
    }
//---------------------------------------------------------------------------------------------------------------------
    void Project::setProcessOutputCallback(std::function <void(std::string const&)> const& cb)
    {
        impl_->cb = cb;
    }
//#####################################################################################################################
}
