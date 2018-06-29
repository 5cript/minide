#pragma once

#include <tiny-process-library/process.hpp>
#include <string>
#include <thread>
#include <memory>
#include <unordered_map>

namespace MinIDE
{
    class AsyncProcess
    {
    public:
        AsyncProcess(
            std::string const& command,
            std::string const& directory,
            std::unordered_map <std::string, std::string> const& environment,
            std::function <void(std::string const&)> cb
        );
        ~AsyncProcess();
    private:
        std::thread thread_;
        std::unique_ptr <TinyProcessLib::Process> process_;
        std::function <void(std::string const&)> cb_;
        int exitStatus_;
    };
}
