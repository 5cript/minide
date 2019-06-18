#include "wizard.hpp"
#include "../common_state_setup.hpp"

#include <sol/sol.hpp>

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
    std::vector <std::pair <std::string, std::string>> Wizard::getFilters()
    {
        sol::state lua;
        commonStateSetup(lua);
        lua.open_libraries(sol::lib::table, sol::lib::math);

        lua["debugging"] = false;

        lua.script(script());
        sol::protected_function getType = lua["getType"];
        if (!getType.valid())
            return {};

        std::string type = getType();
        if (type != "file")
            return {};

        std::vector <std::pair <std::string, std::string>> result;
        sol::protected_function getFilters = lua["getFilters"];
        if (!getFilters.valid())
            return {};

        sol::table filters = getFilters();
        for (auto const& [index, value] : filters)
        {
            sol::table filter = value;

            result.push_back(std::make_pair(
                filter["name"].get <std::string>(),
                filter["filter"].get <std::string>()
            ));
        }
        return result;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::vector <Wizard::Creation> Wizard::runWizard(Parameters const& params)
    {
        sol::state lua;
        commonStateSetup(lua);
        lua.open_libraries(sol::lib::table, sol::lib::math);

        lua["debugging"] = false;

        sol::table parameterTable = lua.create_table();
        for (auto const& [key, param] : params)
        {
            sol::table parameter = lua.create_table();
            parameter["prettyName"] = param.prettyName;
            parameter["value"] = param.defaultValue;
            parameter["description"] = param.description;
            parameter["isOptional"] = param.isOptional;
            parameter["orderHint"] = param.orderHint;
            parameterTable[key] = parameter;
        }

        lua.script(script());
        std::string type = lua["getType"]();

        if (type == "directory")
        {
            std::vector <std::pair <int, Creation>> creas;
            sol::table creatables = lua["runWizard"](parameterTable);
            for (auto const& [index, value] : creatables)
            {
                sol::table crea = value;

                Creation c {
                    crea["name"].get <std::string>(),
                    crea["content"].get <std::string>(),
                    crea["type"].get <std::string>()
                };

                creas.push_back({index.as <int>(), c});
            }
            std::sort(std::begin(creas), std::end(creas), [](auto const& lhs, auto const& rhs) {
                return lhs.first < rhs.first;
            });

            std::vector <Creation> unzipped;
            for (auto const& c : creas) // now sorted
                unzipped.push_back(c.second);

            return unzipped;
        }
        else if (type == "file")
        {
            std::string content = lua["runWizard"](parameterTable);
            Creation file;
            file.content = content;
            return {file};
        }
        else
            throw std::runtime_error("Unexpected wizard type.");
    }
//#####################################################################################################################
}
