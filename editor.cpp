#include "editor.hpp"

#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>

namespace MinIDE
{
//#####################################################################################################################
    struct EditorImpl
    {
        EditorImpl(nana::panel<true>& parent);

        nana::tabbar <std::string> tabs;
        nana::textbox textbox;
        nana::place layout;
    };
//---------------------------------------------------------------------------------------------------------------------
    EditorImpl::EditorImpl(nana::panel<true>& parent)
        : tabs{parent}
        , textbox{parent}
        , layout{parent}
    {

    }
//#####################################################################################################################
    Editor::Editor(nana::window parent)
        : nana::panel <true>{parent}
        , elements_{new EditorImpl(*this)}
    {
        setLayout();
    }
//---------------------------------------------------------------------------------------------------------------------
    Editor::~Editor() = default;
//---------------------------------------------------------------------------------------------------------------------
    void Editor::loadTheme(Theme const& theme)
    {
        elements_->textbox.bgcolor(theme.largeBackgrounds);
        elements_->textbox.fgcolor(theme.textForegrounds);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::setLayout()
    {
        elements_->layout.div(layoutString);
        elements_->layout["tabbar"] << elements_->tabs;
        elements_->layout["editor"] << elements_->textbox;
        elements_->layout.collocate();
    }
//#####################################################################################################################
}
