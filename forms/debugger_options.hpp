#pragma once

#include "../global_settings/global_persistence.hpp"
#include <nana/gui/widgets/form.hpp>
#include <memory>

namespace MinIDE
{
    struct DebuggerOptionsImpl;

    class DebuggerOptions
    {
    public:
        constexpr static const char* layoutString =
            #include "layouts/debugger_options.layout"
        ;

    public:
        DebuggerOptions(
            nana::window owner,
            std::unordered_map <std::string, DebuggerSettings>* settings,
            std::function <void()> saveFunction
        );
        ~DebuggerOptions();

        void show();

    private:
        void setLayout();
        void setupEvents();
        bool promptSave(bool hasCancel = true);
        void loadProfile();
        void disableAllOrSelect();
        void loadProfiles();
        void enableAll();
        bool isDirty();
        void resetDirty();

    private:
        std::unique_ptr <DebuggerOptionsImpl> elements_;
    };
}
