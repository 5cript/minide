#pragma once

#include "../../global_settings/debugger_settings.hpp"
#include <nana/gui/widgets/panel.hpp>

#include <memory>

namespace MinIDE::DebuggerOptionFrames
{
    struct GdbFrameImpl;

    class GdbFrame : public nana::panel <true>
    {
    public:
        constexpr static const char* layoutString =
            #include "../layouts/debugger_option_frames/gdb.layout"
        ;

    public:
        GdbFrame(nana::window parent, DebuggerSettings* settings);
        ~GdbFrame();

        /// Load UI element values from settings container
        void loadFromSettings();
        void enabled(bool enable);
        bool isDirty() const;
        void resetDirty();

    private:
        void setLayout();
        void setupEvents();

    private:
        std::unique_ptr <GdbFrameImpl> elements_;
    };
}
