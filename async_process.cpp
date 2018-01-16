#include "async_process.hpp"

#include <iostream>
#include <chrono>

using namespace std::string_literals;
using namespace std::chrono_literals;

namespace MinIDE
{
//#####################################################################################################################
    AsyncProcess::AsyncProcess(std::string const& command, std::string const& directory, std::function <void(std::string const&)> cb)
        : cb_{std::move(cb)}
    {
        thread_ = std::thread([this, command, directory]()
        {
            std::cout << command << "\n";
            cb_(command + "\n");

            process_ = std::make_unique <TinyProcessLib::Process> (
                command,
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
            auto exitMessage = "Process exited with status "s + std::to_string(exitStatus_) + "\n";
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
