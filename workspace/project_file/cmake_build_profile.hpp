#pragma once

#include "build_profile.hpp"

namespace MinIDE::ProjectPersistence
{
    struct CMakeBuildProfile : BuildProfile
                             , public JSON::Stringifiable <CMakeBuildProfile>
                             , public JSON::Parsable <CMakeBuildProfile>

    {
        /**
         *  @param name = The name of the profile (e.g. "Debug").
         *  @param outputPath = The path to build stuff in(, where the lib/executable will reside in, depends on cmake)
         *  @param outputIsRelative = The outputPath is relative to the project root.
         *  @param environment = The environment settings to use.
         *  @param toolProfile = what tooling to use.
         *  @param executable = What is the executable be called after build (optional, might be library and not runnable).
         *  @param isDebugable = Can this target be debugged?
         *  @param cmakeOptions = Additional cmake options related to this project.
         */
        CMakeBuildProfile(
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
            bool executionDirectoryIsRelative = false,
            std::optional <std::string> cmakeOptions = std::nullopt,
            std::optional <std::string> makeOptions = std::nullopt
        );

        std::optional <std::string> cmakeOptions;
        std::optional <std::string> makeOptions;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::ProjectPersistence::CMakeBuildProfile,
    name, outputPath, outputIsRelative, executable, arguments, isDebugable, debuggerSettings, executionDirectory, environment, toolProfile,
    cmakeOptions, makeOptions, executionDirectoryIsRelative
)
