#pragma once

#include "../global_settings/global_persistence.hpp"
#include <nana/gui/widgets/form.hpp>
#include <memory>
#include <functional>

namespace MinIDE
{
    struct EnvironmentOptionsImpl;

    class EnvironmentOptions
    {
    public:
        constexpr static const char* layoutString =
            #include "layouts/environment_options.layout"
        ;

    public:
        EnvironmentOptions(
            nana::window owner,
            std::unordered_map <std::string, Environment>* environments,
            std::function <void()> saveFunction,
            bool noProfiles = false
        );
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
