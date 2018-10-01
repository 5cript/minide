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
            bool isDebugable = false,
            std::optional <DebuggerSettings> debuggerSettings = std::nullopt
        );

        std::string name;
        std::string outputPath;
        bool outputIsRelative;
        std::string environment;
        std::string toolProfile;
        std::optional <std::string> executable;
        bool isDebugable;
        std::optional <DebuggerSettings> debuggerSettings;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::BuildProfile,
    name, outputPath, outputIsRelative, executable, isDebugable, environment, toolProfile, debuggerSettings
)
