#pragma once

#include "../../filesystem.hpp"

#include <string>
#include <vector>

namespace MinIDE::Breakpoints
{
    enum class Type
    {
        Breakpoint,
        Tracepoint,
        Catchpoint,
        Watchpoint
        // more...?
    };

    enum class Disposition
    {
        Delete,
        Keep
    };

    enum class Evaluator
    {
        Host,
        Target
    };

    struct Breakpoint
    {
        std::pair <int, int> number;
        BreakpointType type;
        Disposition disp;
        std::string catch_type;
        bool enabled;
        std::string address; // addr
        std::string function; // func
        std::string file; // short file representation (usually what was passed on setting the breakpoint, so not necessarily short)
        path fullname; // full file representation.
        int line;
        std::string at; // provided if no file.
        std::string pending;
        Evaluator evaluatedBy;
        std::string thread;
        std::string task; // not relevant for C++
        std::string conditionExpression;
        int ignoreCount; // ignore
        int enableCount; // enable
        std::string static_tracepoint_marker_string_id;
        std::string mask; // for masked watchpoints
        int passCount; // pass
        std::string originalLocation;
        int times; // times breakpoint was hit.
        bool installed;
        std::vector <std::string> thread_groups;

        // what - type dependent data. probably exception stuff
    };
}
