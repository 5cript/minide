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
        boost::optional <std::string> executable,
        bool isDebugable
    )
        : name{name}
        , outputPath{outputPath}
        , outputIsRelative{outputIsRelative}
        , environment{environment}
        , toolProfile{toolProfile}
        , executable{executable}
        , isDebugable{isDebugable}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
