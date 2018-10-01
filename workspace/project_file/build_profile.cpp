#include "build_profile.hpp"

namespace MinIDE::ProjectPersistence
{
//#####################################################################################################################
    BuildProfile::BuildProfile(
        std::string name,
        std::string outputPath,
        bool outputIsRelative,
        std::string environment,
        std::string toolProfile,
        std::optional <std::string> executable,
        bool isDebugable,
        std::optional <DebuggerSettings> debuggerSettings
    )
        : name{name}
        , outputPath{outputPath}
        , outputIsRelative{outputIsRelative}
        , environment{environment}
        , toolProfile{toolProfile}
        , executable{executable}
        , isDebugable{isDebugable}
        , debuggerSettings{debuggerSettings}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
