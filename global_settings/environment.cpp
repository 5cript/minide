#include "environment.hpp"

#include "../filesystem.hpp"

#include <set>

namespace MinIDE
{
//#####################################################################################################################
    std::unordered_map <std::string, std::string> Environment::compile() const
    {
        std::unordered_map <std::string, std::string> env;
        env["PATH"] = path;
        env.insert(std::begin(variables), std::end(variables));
        return env;
    }
//---------------------------------------------------------------------------------------------------------------------
    Environment Environment::merge(Environment const& other)
    {
        Environment merged;
        merged.variables = variables;
        for (auto const& [key, value] : other.variables)
            merged.variables[key] = value;

        auto pathStringToSet = [](std::string const& pathString)
        {
            using MinIDE::path;
            std::set <path> paths;
            std::string singlePath;
            for (auto const i : pathString)
            {
                if (i == ';')
                {
                    paths.insert(path{singlePath}.make_preferred());
                    singlePath.clear();
                }
                else
                    singlePath.push_back(i);
            }
            if (!singlePath.empty())
                paths.insert(path{singlePath}.make_preferred());

            return paths;
        };

        auto mergedPath = pathStringToSet(path);
        mergedPath.merge(pathStringToSet(other.path));
        for (auto const& i : mergedPath)
        {
            merged.path += i.string();
            merged.path.push_back(';');
        }
        merged.path.pop_back();

        return merged;
    }
//#####################################################################################################################
}
