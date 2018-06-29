#include "async_process.hpp"

#include <boost/format.hpp>

#include <iostream>
#include <chrono>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

using namespace std::string_literals;
using namespace std::chrono_literals;

namespace MinIDE
{
//#####################################################################################################################
    AsyncProcess::AsyncProcess(
        std::string const& command,
        std::string const& directory,
        std::unordered_map <std::string, std::string> const& environment,
        std::function <void(std::string const&)> cb
    )
        : cb_{std::move(cb)}
    {
        thread_ = std::thread([this, command, directory, environment]()
        {
            std::cout << command << "\n";
            cb_(command + "\n");

            process_ = std::make_unique <TinyProcessLib::Process> (
                command,
                environment,
                directory,
                [this](char const* bytes, std::size_t count)
                {
                    auto str = std::string{bytes, count};
                    cb_(str);
                    std::cout << str;
                },
                [this](char const* bytes, std::size_t count)
                {
                    auto str = std::string{bytes, count};
                    cb_(str);
                    std::cerr << str;
                },
                true
            );

            auto exitStatus_ = process_->get_exit_status();
            auto exitMessage = "Process exited with status "s + std::to_string(exitStatus_) + " (hex 0x" + (boost::format("%x") % exitStatus_).str() + ")\n";
            #ifdef _WIN32
            if (exitStatus_ == -1)
            {
                exitMessage += "System error: " + std::to_string(GetLastError()) + "\n";
            }
            #endif
            cb_(exitMessage);
            std::cout << exitMessage << "\n";
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    AsyncProcess::~AsyncProcess()
    {
        if (thread_.joinable())
            thread_.join();
    }
//#####################################################################################################################
}
