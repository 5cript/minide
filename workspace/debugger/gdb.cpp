#include "gdb.hpp"

#include <gdb-interface/gdb_run_arguments.hpp>
#include <gdb-interface/gdb.hpp>

#include <thread>

namespace MinIDE
{
//#####################################################################################################################
    namespace
    {
        GdbInterface::GdbRunArguments synthesizeArguments
        (
            path const& projectRoot,
            DebuggerSettings const& debuggerSettings,
            MinIDE::ProjectPersistence::BuildProfile const* target,
            Environment const& targetEnvironment
        )
        {
            if (!debuggerSettings.runArguments)
            throw std::runtime_error("Passed empty run arguments for gdb.");

            auto userArgs = debuggerSettings.runArguments.value();
            auto userArgsDefault = GdbSettings::makeDefault();

            GdbInterface::GdbRunArguments runArgs;

            #define SET_OR_DEFAULT(lhs, rhs) \
            if (userArgs.rhs) \
                runArgs.lhs = userArgs.rhs.value(); \
            else if (userArgsDefault.rhs)\
                runArgs.lhs = userArgsDefault.rhs.value()

            SET_OR_DEFAULT(gdbFullPath, gdbPath);

            auto buildDir = path{};
            if (target->outputIsRelative)
                buildDir = projectRoot / target->outputPath;
            else
                buildDir = target->outputPath;

            auto executable = buildDir / target->executable.value();
            runArgs.program = executable.string();

            auto execDir = path{};
            if (target->executionDirectory)
            {
                if (target->executionDirectoryIsRelative)
                    execDir = projectRoot / target->executionDirectory.value();
                else
                    execDir = target->executionDirectory.value();
            }
            else
                execDir = buildDir;

            runArgs.directory = execDir.string();
            runArgs.pid = std::nullopt;
            runArgs.args = target->arguments;
            runArgs.core = std::nullopt;
            runArgs.sourceDirectory = projectRoot.string();
            runArgs.symbols = std::nullopt;
            SET_OR_DEFAULT(fullyReadSymbols, fullyReadSymbols);
            SET_OR_DEFAULT(neverReadSymbols, neverReadSymbols);
            runArgs.write = false;
            runArgs.quiet = false;
            runArgs.returnChildResult = true;
            SET_OR_DEFAULT(gdbDataDirectory, dataDirectory);
            SET_OR_DEFAULT(initCommandFile, initCommandFile);
            SET_OR_DEFAULT(commandFile, commandFile);
            SET_OR_DEFAULT(ignoreHomeGdbInit, ignoreHomeGdbInit);
            SET_OR_DEFAULT(ignoreAllGdbInit, ignoreAllGdbInit);

            // TODO: Environment
            auto environment = targetEnvironment;
            if (userArgs.environment)
            {
                if (userArgs.environment.value().environmentIsAdditive)
                    environment = environment.merge(userArgs.environment.value().environment);
                else
                    environment = userArgs.environment.value().environment;
            }
            runArgs.environment = environment.compile();

            #undef SET_OR_DEFAULT

            return runArgs;
        }
    }
//#####################################################################################################################
    struct GdbImpl
    {
        GdbImpl(GdbCommunicator* owner, GdbCommunicator::event_manager_type const* eventManager);

        GdbCommunicator* owner;
        std::unique_ptr <GdbInterface::Gdb> gdb;
        std::unique_ptr <Listener> eventListener;
        std::thread thread;
        bool wasStopped;
        bool isRunning;
        GdbCommunicator::event_manager_type const* eventManager;
    };
//#####################################################################################################################
    struct Listener : GdbInterface::PickyListener
    {
        GdbCommunicator* owner;
        Listener(GdbCommunicator* owner)
            : owner{owner}
        {
        }

        void onRawData(std::string const& raw) override
        {
            std::cout << raw;
        }
        void onConsoleStream(std::string const& text) override
        {
            owner->impl_->eventManager->callNamed(ProcessOutput, "debugger_output", text);
        }
        void onTargetStream(std::string const& text) override
        {
            owner->impl_->eventManager->callNamed(ProcessOutput, "program_output", text);
        }
        void onLogStream(std::string const& text) override
        {
            owner->impl_->eventManager->callNamed(ProcessOutput, "debugger_output", text);
        }
        void onParserError(std::string const& text) override
        {
            owner->impl_->eventManager->callNamed(ProcessOutput, "info", text);
        }
        void onStdErr(std::string const& text) override
        {
            owner->impl_->eventManager->callNamed(ProcessOutput, "debugger_output", text);
        }
    };
//#####################################################################################################################
    GdbImpl::GdbImpl(GdbCommunicator* owner, GdbCommunicator::event_manager_type const* eventManager)
        : owner{owner}
        , gdb{}
        , eventListener{new Listener(owner)}
        , thread{}
        , wasStopped{false}
        , isRunning{false}
        , eventManager{eventManager}
    {

    }
//#####################################################################################################################
    GdbCommunicator::GdbCommunicator(GdbCommunicator::event_manager_type const* eventManager)
        : impl_{new GdbImpl(this, eventManager)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    GdbCommunicator::~GdbCommunicator() = default;
//---------------------------------------------------------------------------------------------------------------------
    void GdbCommunicator::run(
        path const& projectRoot,
        DebuggerSettings const& debuggerSettings,
        MinIDE::ProjectPersistence::BuildProfile const* target,
        Environment const& targetEnvironment
    )
    {
        if (impl_->isRunning)
            return;

        auto args = synthesizeArguments(projectRoot, debuggerSettings, target, targetEnvironment);

        impl_->gdb.reset(new GdbInterface::Gdb(args));
        impl_->gdb->registerListener(impl_->eventListener.get());

        if (impl_->thread.joinable())
            impl_->thread.join();
        impl_->thread = std::thread([this]{
            impl_->wasStopped = false;
            impl_->gdb->start();
            impl_->eventManager->callNamed(ProcessExited, "process_exit", static_cast <int> (impl_->gdb->waitForProcess()));
            impl_->isRunning = false;
        });
        impl_->isRunning = true;
    }
//---------------------------------------------------------------------------------------------------------------------
    void GdbCommunicator::stop()
    {
        impl_->gdb->stop();
        impl_->wasStopped = true;
    }
//---------------------------------------------------------------------------------------------------------------------
    void GdbCommunicator::kill()
    {
        impl_->gdb->forceKill();
    }
//---------------------------------------------------------------------------------------------------------------------
    bool GdbCommunicator::wasStopped() const noexcept
    {
        return impl_->wasStopped;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool GdbCommunicator::isRunning() const noexcept
    {
        return impl_->isRunning;
    }
//#####################################################################################################################
}
