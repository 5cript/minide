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
        std::optional <std::string> executable,
        std::optional <std::string> arguments,
        bool isDebugable,
        std::optional <DebuggerSettings> debuggerSettings,
        std::optional <std::string> executionDirectory,
        bool executionDirectoryIsRelative,
        std::optional <std::string> cmakeOptions,
        std::optional <std::string> makeOptions
    )
        : BuildProfile{
            std::move(name),
            std::move(outputPath),
            std::move(outputIsRelative),
            std::move(environment),
            std::move(toolProfile),
            std::move(executable),
            std::move(arguments),
            std::move(isDebugable),
            std::move(debuggerSettings),
            std::move(executionDirectory),
            std::move(executionDirectoryIsRelative)
        }
        , cmakeOptions{std::move(cmakeOptions)}
        , makeOptions{std::move(makeOptions)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
