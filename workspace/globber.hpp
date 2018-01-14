#pragma once

#include "../filesystem.hpp"

#include <string>

namespace MinIDE
{
    class Globber
    {
    public:
        /**
         *  @param root The root directory to glob files (or dirs) in.
         *  @param directories Collect directories instead of files if true.
         */
        Globber(std::string root, bool directories = false);

        void setBlackList(std::vector <std::string> const& blackList);
        void setDirectoryBlackList(std::vector <std::string> const& blackList);

        std::vector <path> glob(std::string const& mask, bool prependRoot);
        std::vector <path> globRecursive(std::string const& mask, bool prependRoot);

        std::vector <path> glob(std::vector <std::string> const& masks, bool prependRoot);
        std::vector <path> globRecursive(std::vector <std::string> const& masks, bool prependRoot);

    private:
        template <typename IteratorT>
        void globImpl(IteratorT& i, std::vector <path>& result, std::vector<std::string> const& masks, bool prependRoot)
        {
            if (!exists(i->status()))    // does p actually exist?
                return;

            bool cont = false;
            if (!directories_ && is_regular_file(i->status()))
                cont = true;
            else if (directories_ && is_directory(i->status()))
                cont = true;

            if (!cont)
                return;

            auto pathTemp = boost::filesystem::relative(i->path(), root_).string();
            std::replace(pathTemp.begin(), pathTemp.end(), '\\', '/');
            auto p = path{pathTemp};

            if (isBlacklisted(p))
                return;

            bool anyMask = masks.empty();
            for (auto const& mask : masks)
            {
                if (checkMask(p, mask))
                {
                    anyMask = true;
                    break;
                }
            }
            if (!anyMask)
                return;

            if (prependRoot)
                result.push_back(root_ / p);
            else
                result.push_back(p);
        }

        /**
         *  Check for a *? wildcard match.
         */
        bool checkMask(path const& p, std::string const& mask);

        /**
         *  Returns whether the path is within the black list or not.
         *
         *  @return Return true if path is BLACKLISTED.
         */
        bool isBlacklisted(path const& p);

    private:
        path root_;
        std::vector <std::string> blackList_;
        std::vector <std::string> dirBlackList_;
        bool directories_;
    };
}
