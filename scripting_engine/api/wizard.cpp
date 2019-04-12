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
        auto retVal = getParameters();

        return {};
    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
