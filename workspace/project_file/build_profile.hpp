#pragma once

#include "../../json_public.hpp"

#include "../../global_settings/debugger_settings.hpp"
#include <string>
#include <optional>

namespace MinIDE::ProjectPersistence
{
    struct BuildProfile // NO INHERITANCE
    {
        virtual ~BuildProfile() = default;
        BuildProfile(
            std::string name = "",
            std::string outputPath = "",
            bool outputIsRelative = true,
            std::string environment = "",
            std::string toolProfile = "",
            std::optional <std::string> executable = std::nullopt,
            std::optional <std::string> arguments = std::nullopt,
            bool isDebugable = false,
            std::optional <DebuggerSettings> debuggerSettings = std::nullopt,
            std::optional <std::string> executionDirectory = std::nullopt,
            bool executionDirectoryIsRelative = false
        );

        std::string name;
        std::string outputPath;
        bool outputIsRelative;
        std::string environment;
        std::string toolProfile;
        std::optional <std::string> executable;
        std::optional <std::string> arguments;
        bool isDebugable;
        std::optional <DebuggerSettings> debuggerSettings;
        std::optional <std::string> executionDirectory;
        bool executionDirectoryIsRelative;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::BuildProfile,
    name, outputPath, outputIsRelative, executable, arguments, isDebugable, environment, toolProfile, debuggerSettings,
    executionDirectory, executionDirectoryIsRelative
)
