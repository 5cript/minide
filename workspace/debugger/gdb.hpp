#pragma once

#include "../project_file/build_profile.hpp"

#include "../../global_settings/debugger_settings.hpp"
#include "../../global_settings/environment.hpp"

#include "../../event_manager.hpp"
#include "../project_events.hpp"
#include "../../filesystem.hpp"

#include <gdb-interface/gdb_interface.hpp>

#include <memory>

namespace MinIDE
{
    struct GdbImpl;
    struct Listener;

    class GdbCommunicator
    {
        friend Listener;

    public:
        using event_manager_type = ProjectEventManagerType;

        GdbCommunicator(event_manager_type const* eventManager);
        ~GdbCommunicator();

        void run
        (
            path const& projectRoot,
            DebuggerSettings const& debuggerSettings,
            MinIDE::ProjectPersistence::BuildProfile const* target,
            Environment const& targetEnvironment
        );

        void stop();
        void kill();

        /// returns whether stop was called.
        bool wasStopped() const noexcept;

        /// returns whether gdb is running.
        bool isRunning() const noexcept;

    private:
        std::unique_ptr <GdbImpl> impl_;
    };
}
