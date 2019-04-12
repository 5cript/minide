#pragma once

#include "../global_settings/global_persistence.hpp"

#include <nana/gui/widgets/form.hpp>
#include <memory>

namespace MinIDE
{
    struct ToolOptionsImpl;

    class ToolOptions
    {
    public:
        ToolOptions(nana::window owner, GlobalPersistence* settings);
        ~ToolOptions();

        void show();

    private:
        void setupLayout();
        void setupEvents();
        void promptSave();
        void loadProfile();
        void disableAllOrSelect();
        void loadProfiles();
        void enableAll();

    private:
        std::unique_ptr <ToolOptionsImpl> elements_;
    };
}
