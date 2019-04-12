#include "wizard.hpp"
#include "../common_state_setup.hpp"

#include <sol.hpp>

namespace MinIDE::Scripting::Api
{
//#####################################################################################################################
    Wizard::Parameters Wizard::retrieveParameters()
    {
        sol::state lua;
        commonStateSetup(lua);
        lua.open_libraries(sol::lib::table, sol::lib::math);

        lua["debugging"] = false;

        lua.script(script());

        auto getParameters = lua["getParameters"];
        sol::table retVal = getParameters();

        Parameters params;
        for (auto const& [key, value] : retVal)
        {
            params[key.as <std::string>()] =
            {
                key.as <std::string>(),
                value.as <std::string>(),
                ""
            };
        }

        sol::table descriptions = lua["getDescriptions"]();
        for (auto const& [key, value] : retVal)
        {
            params[key.as <std::string>()].description = value.as <std::string>();
        }

        return params;
    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
