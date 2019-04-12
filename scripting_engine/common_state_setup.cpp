#include "common_state_setup.hpp"

#include "../resources.hpp"

namespace MinIDE::Scripting
{
//#####################################################################################################################
    void commonStateSetup(sol::state& state)
    {
        state.open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::table,
            sol::lib::math,
            sol::lib::string,
            sol::lib::io,
            sol::lib::coroutine,
            sol::lib::debug
        );

        const std::string package_path = state["package"]["path"];
        std::string path = state["package"]["path"];
        path = path + (!path.empty() ? ";" : "") + (getResourcesDirectory() / "lib" / "Penlight" / "lua" / "?.lua").string();
        state["package"]["path"] = path;
    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
