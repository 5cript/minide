#pragma once

#include "filesystem.hpp"

namespace MinIDE
{
    /**
     *  Load a resource file from HDD relative to the resources folder.
     *  Loads the respective file into a string and returns it.
     *  Dont load big files like this for obvious reasons
     */
    std::string loadResource(path const& file);

    /**
     *  Does not load a resource but creates a path relative to the resource directory and returns it.
     */
    path resource(path const& file);

    /**
     *  Returns an absolute path to the resources dir.
     */
    path getResourcesDirectory();
}
