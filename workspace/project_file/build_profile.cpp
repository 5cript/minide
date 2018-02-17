#include "build_profile.hpp"

namespace MinIDE::ProjectPersistence
{
//#####################################################################################################################
    BuildProfile::BuildProfile(
        std::string name,
        std::string outputPath,
        std::string environment,
        std::string toolProfile,
        boost::optional <std::string> executable,
        bool isDebugable
    )
        : name{name}
        , outputPath{outputPath}
        , environment{environment}
        , toolProfile{toolProfile}
        , executable{executable}
        , isDebugable{isDebugable}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
