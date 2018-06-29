#include "cmake_build_profile.hpp"

namespace MinIDE::ProjectPersistence
{
//#####################################################################################################################
    CMakeBuildProfile::CMakeBuildProfile(
        std::string name,
        std::string outputPath,
        bool outputIsRelative,
        std::string environment,
        std::string toolProfile,
        boost::optional <std::string> executable,
        bool isDebugable,
        boost::optional <std::string> cmakeOptions,
        boost::optional <std::string> makeOptions
    )
        : BuildProfile{
            std::move(name),
            std::move(outputPath),
            std::move(outputIsRelative),
            std::move(environment),
            std::move(toolProfile),
            std::move(executable),
            std::move(isDebugable)
        }
        , cmakeOptions{std::move(cmakeOptions)}
        , makeOptions{std::move(makeOptions)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
