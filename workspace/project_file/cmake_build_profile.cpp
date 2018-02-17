#include "cmake_build_profile.hpp"

namespace MinIDE::ProjectPersistence
{
//#####################################################################################################################
    CMakeBuildProfile::CMakeBuildProfile(
        std::string name,
        std::string outputPath,
        std::string environment,
        std::string toolProfile,
        boost::optional <std::string> executable,
        bool isDebugable,
        boost::optional <std::string> cmakeOptions
    )
        : BuildProfile{
            name, outputPath, environment, toolProfile, executable, isDebugable
        }
        , cmakeOptions{cmakeOptions}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
