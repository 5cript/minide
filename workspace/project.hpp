#pragma once

#include "../filesystem.hpp"

namespace MinIDE
{
    class Project
    {
    public:
        virtual ~Project() = default;

        virtual void loadEnvironment(path const& rootDir) = 0;

    protected:

    private:

    };
}
