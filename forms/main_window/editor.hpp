#pragma once

#include "../../filesystem.hpp"
#include "../../copyability.hpp"
#include "../../theme.hpp"
#include "highlighting.hpp"

#include "../../signals.hpp"

#include <nana/gui/widgets/panel.hpp>
#include <memory>
#include <functional>

namespace MinIDE
{
    struct EditorImpl;

    class Editor : public nana::panel<true>
    {
    public:
        constexpr static const char* layoutString =
            "<vertical margin=0 <tabbar max=18><<weight=45 lines> editor>>"
        ;

        NONCOPY(Editor);

    public:
        Editor(nana::window parent);
        ~Editor();

        void loadTheme(Theme const& theme);
        void loadFile(path const& projectRoot, path const& relativeToRootName);
        void focusTextbox();
        void save();
        void saveAll();

    public: // SIGNALS
        sig2::signal<void(path const&, int, nana::arg_mouse const&)> sidepanelClickEvent;

    private:
        void setLayout();
        void tabSynchronize();
        void registerTabEvents();
        void registerEditorEvents();
        void registerNumberPaneEvents();
        void setText(std::string const& text);
        void setEditorEnabled(bool enabled);
        void setHighlighting(Highlighting const& highlight);
        void onTextboxKeyRelease(nana::arg_keyboard const& kbEvent);

    private:
        std::unique_ptr <EditorImpl> elements_;
    };
}
