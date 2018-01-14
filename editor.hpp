#pragma once

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

    private:
        void setLayout();

    private:
        std::unique_ptr <EditorImpl> elements_;
    };
}
