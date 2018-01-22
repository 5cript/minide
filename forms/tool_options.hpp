#pragma once

#include "../global_settings/global_persistence.hpp"
#include <memory>

namespace MinIDE
{
    struct ToolOptionsImpl;

    class ToolOptions
    {
    public:
        constexpr static const char* layoutString =
            #include "layouts/ToolOptions.layout"
        ;

    public:
        ToolOptions(GlobalPersistence* settings);
        ~ToolOptions();

        void show();

    private:
        std::unique_ptr <ToolOptionsImpl> elements_;
    };
}
