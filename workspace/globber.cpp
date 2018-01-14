#include "globber.hpp"

#include <functional>

namespace MinIDE
{
//#####################################################################################################################
    template <typename IteratorT>
    void globImpl(Globber* globber, IteratorT& i, std::vector <path>& result)
    {
    }
//#####################################################################################################################
    Globber::Globber(std::string root, bool directories)
        : root_{std::move(root)}
        , blackList_{}
        , dirBlackList_{}
        , directories_{directories}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void Globber::setBlackList(std::vector <std::string> const& blackList)
    {
        blackList_ = blackList;
    }
//---------------------------------------------------------------------------------------------------------------------
    void Globber::setDirectoryBlackList(std::vector <std::string> const& blackList)
    {
        dirBlackList_ = blackList;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool Globber::isBlacklisted(path const& p)
    {
        for (auto const& i : blackList_)
            if (checkMask(p, i))
                return true;
        for (auto const& i : dirBlackList_)
        {
            auto root = p;
            while (!root.parent_path().empty())
                root = root.parent_path();
            if (root.string() == i)
                return true;
        }
        return false;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <path> Globber::glob(std::string const& mask, bool prependRoot)
    {
        return glob({mask}, prependRoot);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <path> Globber::globRecursive(std::string const& mask, bool prependRoot)
    {
        return globRecursive({mask}, prependRoot);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <path> Globber::glob(std::vector <std::string> const& masks, bool prependRoot)
    {
        using namespace boost::filesystem;
        std::vector <path> files;
        for (directory_iterator i{root_}, end; i != end; ++i)
            globImpl(i, files, masks, prependRoot);
        return files;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <path> Globber::globRecursive(std::vector <std::string> const& masks, bool prependRoot)
    {
        using namespace boost::filesystem;
        std::vector <path> files;
        for (recursive_directory_iterator i{root_}, end; i != end; ++i)
            globImpl(i, files, masks, prependRoot);
        return files;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool Globber::checkMask(path const& p, std::string const& mask)
    {
        std::function <bool(const char*, const char*)> match;

        match = [&match](char const *needle, char const *haystack) -> bool
        {
            for (; *needle != '\0'; ++needle)
            {
                switch (*needle)
                {
                    case '?':
                        if (*haystack == '\0')
                            return false;
                        ++haystack;
                        break;
                    case '*':
                    {
                        if (needle[1] == '\0')
                            return true;
                        size_t max = strlen(haystack);
                        for (size_t i = 0; i < max; i++)
                            if (match(needle + 1, haystack + i))
                                return true;
                        return false;
                    }
                    default:
                        if (*haystack != *needle)
                            return false;
                        ++haystack;
                }
            }
            return *haystack == '\0';
        };

        return match(mask.c_str(), p.string().c_str());
    }
//#####################################################################################################################
}
