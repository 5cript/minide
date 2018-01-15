#pragma once

#include "../settings/settings.hpp"
#include "../filesystem.hpp"

namespace MinIDE
{
    class Project
    {
    public:
        Project(Settings* settings);
        virtual ~Project() = default;

        virtual void loadEnvironment(path const& rootDir) = 0;

        /**
         *  Different project types probably need a different amount of build steps.
         */
        virtual void buildStep(int step, bool debug) = 0;

        path rootDir() const;
        std::string name() const;
        std::vector <path> const* files() const;
        std::vector <path> const* directories() const;
        void setProcessOutputCallback(std::function <void(std::string const&)> const& cb);

    protected:
        void glob(
            path const& rootDir,
            std::vector <std::string> const& masks,
            std::vector <std::string> const& directoryBlackList
        );

    protected:
        Settings* settings_;
        path rootDir_;
        std::vector <path> files_;
        std::vector <path> directories_;
        std::function <void(std::string const&)> cb_;
    };
}
