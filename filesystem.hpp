#pragma once

#include <boost/filesystem.hpp>

#include <stdexcept>

namespace MinIDE
{
    class jailbreak_error : public std::runtime_error
    {
    public:
        jailbreak_error(const char* c)
            : std::runtime_error(c)
        {
        }
    };

    namespace filesystem = boost::filesystem;
    using path = filesystem::path;

    path jail(path const& root, path const& subDir);
}
