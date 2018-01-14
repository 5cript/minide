#include "editor.hpp"

#include "editor_file_store.hpp"

#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>

#include <iostream>
#include <type_traits>

namespace MinIDE
{
//#####################################################################################################################
    struct EditorImpl
    {
        EditorImpl(nana::panel<true>& parent);

        FileStore fileStore;
        nana::tabbar <int> tabs;
        nana::textbox textbox;
        nana::place layout;

        bool disableEditChangeEvent = false;
    };
//---------------------------------------------------------------------------------------------------------------------
    EditorImpl::EditorImpl(nana::panel<true>& parent)
        : fileStore{}
        , tabs{parent}
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
        registerTabEvents();
        registerEditorEvents();
        elements_->tabs.close_fly(true);
    }
//---------------------------------------------------------------------------------------------------------------------
    Editor::~Editor() = default;
//---------------------------------------------------------------------------------------------------------------------
    void Editor::loadFile(path const& projectRoot, path const& relativeToRootName)
    {
        auto& store = elements_->fileStore;

        if (!store.empty())
        {
            setText(elements_->fileStore.selected().data());
        }

        auto oldSize = store.size();
        auto newIndex = store.add(projectRoot, relativeToRootName);
        if (newIndex == -1)
            return;

        // set editor text
        auto& selected = store.selected();

        // store did not grow
        if (store.size() != oldSize)
            elements_->tabs.append(store.selected().displayName(), nullptr, store.index());

        tabSynchronize();

        //elements_->textbox.load(filename.string());
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::registerTabEvents()
    {
        elements_->tabs.events().activated([this](auto const&)
        {
            setText(elements_->fileStore.selected().data());
        });

        elements_->tabs.events().removed([this](auto const& removed) mutable
        {
            const_cast <std::decay_t<decltype(removed)>&> (removed).remove =
                elements_->fileStore.remove(removed.value)
            ;
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::tabSynchronize()
    {
        auto& store = elements_->fileStore;

        if (store.empty())
            return;

        elements_->tabs.activated(store.index());
        setText(store.selected().data());
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::setText(std::string const& text)
    {
        elements_->disableEditChangeEvent = true;
        elements_->textbox.caption(text);
        elements_->disableEditChangeEvent = false;
    }
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
//---------------------------------------------------------------------------------------------------------------------
    void Editor::registerEditorEvents()
    {
        elements_->textbox.events().text_changed([this](auto const&)
        {
            if (elements_->disableEditChangeEvent)
                return;

            auto& store = elements_->fileStore;

            if (store.empty())
                return;

            store.selected().data(elements_->textbox.caption());

            elements_->tabs.text(store.index(), store.selected().displayName() + "*");
        });
    }
//#####################################################################################################################
}
