#pragma once

#include "../global_settings/global_persistence.hpp"
#include <nana/gui/widgets/form.hpp>
#include <memory>

namespace MinIDE
{
    struct EnvironmentOptionsImpl;

    class EnvironmentOptions
    {
    public:
        constexpr static const char* layoutString =
            #include "layouts/EnvironmentOptions.layout"
        ;

    public:
        EnvironmentOptions(nana::window owner, GlobalPersistence* settings);
        ~EnvironmentOptions();

        void show();

    private:
        void setupLayout();
        void setupEnvironmentListbox();
        void setupEvents();
        void loadEnvironments();
        void promptSave();
        void loadProfile();
        void disableAllOrSelect();
        void enableAll();
        /**
         *  Format a path for the textbox or back. toMachine = false -> format for human readability.
         */
        std::string formatPath(std::string const& path, bool toMachine);

    private:
        std::unique_ptr <EnvironmentOptionsImpl> elements_;
    };
}
