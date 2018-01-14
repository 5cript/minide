#pragma once

#include "filesystem.hpp"
#include "copyability.hpp"
#include "theme.hpp"

#include <nana/gui/widgets/panel.hpp>
#include <memory>

namespace MinIDE
{
    struct EditorImpl;

    class Editor : public nana::panel<true>
    {
    public:
        constexpr static const char* layoutString =
            "<vertical margin=0 <tabbar max=18><editor>>"
        ;

        NONCOPY(Editor);

    public:
        Editor(nana::window parent);
        ~Editor();

        void loadTheme(Theme const& theme);
        void loadFile(path const& projectRoot, path const& relativeToRootName);

    private:
        void setLayout();
        void tabSynchronize();
        void registerTabEvents();
        void registerEditorEvents();
        void setText(std::string const& text);

    private:
        std::unique_ptr <EditorImpl> elements_;
    };
}
