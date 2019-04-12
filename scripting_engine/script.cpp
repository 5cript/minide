#include "script.hpp"

#include <fstream>

namespace MinIDE::Scripting
{
//#####################################################################################################################
    Script::Script(path const& scriptFile)
    {
        std::ifstream reader{scriptFile, std::ios_base::binary};

        reader.seekg(0, std::ios::end);
        script_.resize(reader.tellg());
        reader.seekg(0);
        reader.read(&script_[0], script_.size());
    }
//---------------------------------------------------------------------------------------------------------------------
    Script::Script(std::string string)
        : script_{std::move(string)}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    Script::Script()
        : script_{}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    std::string_view Script::viewScript()
    {
        return script_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string const& Script::script() const
    {
        return script_;
    }
//#####################################################################################################################
}
