#pragma once

#include "../json_public.hpp"
#include "environment.hpp"
#include <gdb-interface/gdb_run_arguments.hpp>
#include <optional>

namespace MinIDE
{
    struct GdbEnvironmentSettings : public JSON::Stringifiable <GdbEnvironmentSettings>
                                  , public JSON::Parsable <GdbEnvironmentSettings>
    {
        /// The environment variables.
        Environment environment;

        /// Add to target environment or override.
        bool environmentIsAdditive;
    };

    struct GdbSettings : public JSON::Stringifiable <GdbSettings>
                       , public JSON::Parsable <GdbSettings>
    {
        std::optional <std::string> gdbPath; // => default: gdb
        std::optional <bool> fullyReadSymbols; // => default: false
        std::optional <bool> neverReadSymbols; // => default: false
        std::optional <bool> returnChildResult; // => default: true
        std::optional <GdbEnvironmentSettings> environment; // => default: nullopt = inherit from IDE
        std::optional <std::string> initCommandFile;
        std::optional <std::string> commandFile;
        std::optional <bool> ignoreHomeGdbInit; // => default: false
        std::optional <bool> ignoreAllGdbInit; // => default: false
    };

    struct DebuggerSettings : public JSON::Stringifiable <DebuggerSettings>
                            , public JSON::Parsable <DebuggerSettings>
    {
        std::optional <GdbSettings> runArguments;
        bool autoRun;
        bool autoWatchLocals;
        bool autoWatchFunctionArgs;
        bool autoBuild;
    };
}


BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::GdbEnvironmentSettings,
    environment, environmentIsAdditive
)

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::GdbSettings,
    gdbPath, fullyReadSymbols, neverReadSymbols, returnChildResult, environment, initCommandFile,
    commandFile, ignoreHomeGdbInit, ignoreAllGdbInit
)

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::DebuggerSettings,
    runArguments, autoRun, autoWatchLocals, autoWatchFunctionArgs, autoBuild
)
