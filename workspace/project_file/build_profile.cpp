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
        std::optional <std::string> arguments,
        bool isDebugable,
        std::optional <DebuggerSettings> debuggerSettings,
        std::optional <std::string> executionDirectory,
        bool executionDirectoryIsRelative
    )
        : name{std::move(name)}
        , outputPath{std::move(outputPath)}
        , outputIsRelative{outputIsRelative}
        , environment{std::move(environment)}
        , toolProfile{std::move(toolProfile)}
        , executable{std::move(executable)}
        , arguments{std::move(arguments)}
        , isDebugable{isDebugable}
        , debuggerSettings{std::move(debuggerSettings)}
        , executionDirectory{std::move(executionDirectory)}
        , executionDirectoryIsRelative{executionDirectoryIsRelative}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
