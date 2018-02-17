#pragma once

#include "../global_settings/global_persistence.hpp"
#include "project_file/project_file.hpp"
#include "project_file/local.hpp"
#include "../filesystem.hpp"

#include "project_file/project_file.hpp"
#include "project_file/local.hpp"

#include <vector>
#include <string>
#include <functional>

namespace MinIDE
{
    struct ProjectImpl
    {
        ProjectImpl(GlobalPersistence* settings);

        GlobalPersistence* settings;
        path rootDir;
        std::vector <path> files;
        std::vector <path> directories;
        std::function <void(std::string const&)> cb;
        ProjectPersistence::ProjectFile projectFile;
        ProjectPersistence::LocalPersistence localFile;
    };
}
