#include "filesystem.hpp"

namespace MinIDE
{
//#####################################################################################################################
    path jail(path const& root, path const& subDir)
    {
        boost::system::error_code ec;
        auto relative = filesystem::relative(root / subDir, root, ec);
        if (ec)
            throw jailbreak_error("path cannot be made relative");
        if (relative.empty())
            throw jailbreak_error("path cannot be made relative");

        for (auto const& part : relative)
        {
            auto str = part.string();

            if (str == "..")

            if (str.substr(0, 2) == "//" || str.substr(0, 2) == "\\\\")
                throw jailbreak_error("relative path may not point to net resource");

            if (str.size() >= 2 && str[1] == ':')
                throw jailbreak_error("relative path contains root");
        }
        return relative;
    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
