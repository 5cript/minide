#include "resources.hpp"

#include <boost/dll/runtime_symbol_info.hpp>

#include <fstream>
#include <sstream>

namespace MinIDE
{
//#####################################################################################################################
    path getResourcesDirectory()
    {
        auto program_path = boost::dll::program_location();
        program_path = program_path.parent_path();

        // for dev:
        auto fname = program_path.filename().string();
        if (fname == "Debug" || fname == "Release")
            program_path = program_path.parent_path();

        auto res = program_path.parent_path() / "resources";

        return res;
    }
//---------------------------------------------------------------------------------------------------------------------
    path resource(path const& file)
    {
        return getResourcesDirectory() / file;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string loadResource(path const& file)
    {
        auto res = resource(file);
        std::ifstream reader{res, std::ios_base::binary};
        std::stringstream buffer;
        buffer << reader.rdbuf();

        return buffer.str();
    }
//#####################################################################################################################
}
