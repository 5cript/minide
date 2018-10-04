#pragma once

#include "../event_manager.hpp"

namespace MinIDE
{
    enum ProjectEvents
    {
        ProcessOutput,
        ProcessExited,
        Info
    };

    using ProjectEventManagerType = EventManagement::EventManager<
        ProjectEvents,
        5,
        int, std::string
    >;
}
