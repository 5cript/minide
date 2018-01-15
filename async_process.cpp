#include "async_process.hpp"

#include <iostream>

namespace MinIDE
{
//#####################################################################################################################
    AsyncProcess::AsyncProcess(std::string const& command, std::string const& directory, std::function <void(std::string const&)> cb)
        : cb_{std::move(cb)}
    {
        thread_ = std::thread([this, command, directory]()
        {
            std::cout << command << "\n";

            process_ = std::make_unique <TinyProcessLib::Process> (
                command,
                directory,
                [this](char const* bytes, std::size_t count)
                {
                    auto str = std::string{bytes, count};
                    cb_(str);
                    std::cout << str << "\n";
                },
                [this](char const* bytes, std::size_t count)
                {
                    auto str = std::string{bytes, count};
                    cb_(str);
                    std::cerr << str << "\n";
                },
                true
            );

            auto exitStatus_ = process_->get_exit_status();
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
