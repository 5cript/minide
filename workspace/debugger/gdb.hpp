#pragma once

#include "../project_file/build_profile.hpp"

#include "../../global_settings/debugger_settings.hpp"
#include "../../global_settings/environment.hpp"

#include "../../event_manager.hpp"
#include "../project_events.hpp"
#include "../../filesystem.hpp"
#include "../../signals.hpp"

#include <gdb-interface/gdb_interface.hpp>
#include <gdb-interface/commands/mi_command.hpp>

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

        /**
         *  Send exit command to gdb for a gentle stop.
         */
        void stop();

        /**
         *  Kill the gdb process.
         */
        void kill();

        /**
         *  returns whether stop was called.
         */
        bool wasStopped() const noexcept;

        /**
         *  returns whether gdb is running.
         */
        bool isRunning() const noexcept;

        /**
         *  Do not use directly.
         */
        void sendCommand(GdbInterface::MiCommand const& command);

        /**
         *  Set/Unset breakpoint in file:line.
         */
        void toggleBreakpoint(path const& file, int line);

        /**
         *  Continue Debugging. (calling it step, because continue is a keyword).
         */
        void step();

    public: // SIGNALS
        sig2::signal <void(path const& file, int line)> onBreakpointSet;

    private:
        std::unique_ptr <GdbImpl> impl_;
    };
}
