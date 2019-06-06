#include "wizard.hpp"
#include "../common_state_setup.hpp"

#include <sol.hpp>

namespace MinIDE::Scripting::Api
{
//#####################################################################################################################
    Wizard::Parameters Wizard::retrieveParameters() const
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
            sol::table tab = value;

            Wizard::Parameter param =
            {
                key.as <std::string>(),
                tab["prettyName"].get <std::string>(),
                tab["value"].get <std::string>(),
                tab["description"].get <std::string>(),
                tab["isOptional"].get <bool>(),
                tab["orderHint"].get <int>()
            };

            params[key.as <std::string>()] = param;
        }

        return params;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Wizard::retrieveType() const
    {
        sol::state lua;
        commonStateSetup(lua);
        lua.open_libraries(sol::lib::table, sol::lib::math);

        lua["debugging"] = false;

        lua.script(script());
        std::string result = lua["getType"]();
        return result;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <Creation> Wizard::runWizard(Parameters const& params, path const& root)
    {

        sol::state lua;
        commonStateSetup(lua);
        lua.open_libraries(sol::lib::table, sol::lib::math);

        lua["debugging"] = false;

        sol::table parameterTable = lua.create_table();
        for (auto const& [key, param] : params)
        {
            sol::table parameter = lua.create_named_table(key,
                "prettyName", param.prettyName,
                "value", param.defaultValue,
                "description", param.description,
                "isOptional", param.isOptional,
                "orderHint", param.orderHint
            );
            parameterTable.add(key, parameter);
        }

        lua.script(script());
        sol::table creatables = lua["runWizard"](parameterTable);

        std::vector <Creation> creas;
        for (auto const& [index, value] : creatables)
        {
            std::cout << index.as <int>() << "\n";
            sol::table crea = value;

            Creation c {
                crea["name"].get <std::string>(),
                crea["content"].get <std::string>(),
                crea["type"].get <std::string>()
            };

            creas.push_back(c);
        }

        return creas;
    }
//#####################################################################################################################
}
