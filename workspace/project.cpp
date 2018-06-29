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
        if (filesystem::exists(localFile))
            impl_->localFile.load(localFile);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Project::saveProjectFiles()
    {
        auto projectFile = impl_->rootDir / ".minproj";
        auto localFile = impl_->rootDir / ".minproj.local";

        impl_->projectFile.save(projectFile);
        impl_->localFile.save(localFile);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <std::string> Project::getBuildTargetNames() const
    {
        std::vector <std::string> targets;
        for (auto const& target : impl_->localFile.buildProfiles)
            targets.push_back(target->name);

        return targets;
    }
//---------------------------------------------------------------------------------------------------------------------
    ProjectPersistence::BuildProfile* Project::getTarget(std::string const& target)
    {
        auto& profiles = impl_->localFile.buildProfiles;
        auto profile = std::find_if(std::begin(profiles), std::end(profiles), [&](auto const& profile){
            return profile->name == target;
        });
        if (profile == std::end(profiles))
            return nullptr;
        else
            return profile->get();
    }
//---------------------------------------------------------------------------------------------------------------------
    void Project::removeTarget(std::string const& targetName)
    {
        auto& profiles = impl_->localFile.buildProfiles;
        profiles.erase(std::remove_if(
            std::begin(profiles),
            std::end(profiles),
            [&](auto const& profile){
                return profile->name == targetName;
            }
        ), std::end(profiles));
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
    GlobalPersistence* Project::getSettings()
    {
        return impl_->settings;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::function <void(std::string const&)> const& Project::callback()
    {
        return impl_->cb;
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
