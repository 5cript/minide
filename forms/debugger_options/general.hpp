#pragma once

#include "../../global_settings/debugger_settings.hpp"
#include <nana/gui/widgets/panel.hpp>

#include <memory>

namespace MinIDE::DebuggerOptionFrames
{
    struct GeneralFrameImpl;

    class GeneralFrame : public nana::panel <true>
    {
    public:
        GeneralFrame(nana::window parent, DebuggerSettings* settings);
        ~GeneralFrame();

        /// Load UI element values from settings container
        void loadFromSettings();
        void enabled(bool enable);
        bool isDirty() const;
        void resetDirty();

    private:
        void setLayout();
        void setupEvents();

    private:
        std::unique_ptr <GeneralFrameImpl> elements_;
    };
}
