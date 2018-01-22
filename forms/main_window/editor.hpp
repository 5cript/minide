#pragma once

#include "../../filesystem.hpp"
#include "../../copyability.hpp"
#include "../../theme.hpp"
#include "highlighting.hpp"

#include <nana/gui/widgets/panel.hpp>
#include <memory>

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

    private:
        void setLayout();
        void tabSynchronize();
        void registerTabEvents();
        void registerEditorEvents();
        void setText(std::string const& text);
        void setEditorEnabled(bool enabled);
        void setHighlighting(Highlighting const& highlight);
        void onTextboxKeyRelease(nana::arg_keyboard const& kbEvent);

    private:
        std::unique_ptr <EditorImpl> elements_;
    };
}
