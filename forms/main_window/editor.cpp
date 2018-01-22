#include "editor.hpp"

#include "editor_file_store.hpp"

#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/drawing.hpp>

#include <iostream>
#include <type_traits>
#include <cctype>

namespace MinIDE
{
//#####################################################################################################################
    struct EditorImpl
    {
        EditorImpl(nana::panel<true>& parent);

        FileStore fileStore;
        nana::tabbar <std::string> tabs;
        nana::textbox textbox;
        nana::place layout;
        nana::panel <true> lineNumbers;
        nana::drawing lineNumberPainter;

        bool disableEditChangeEvent = false;
    };
//---------------------------------------------------------------------------------------------------------------------
    EditorImpl::EditorImpl(nana::panel<true>& parent)
        : fileStore{}
        , tabs{parent}
        , textbox{parent}
        , layout{parent}
        , lineNumbers{static_cast <nana::window> (parent)}
        , lineNumberPainter{lineNumbers}
    {
        using namespace nana;
        lineNumberPainter.draw([&](paint::graphics& graph)
        {
            auto line_px = textbox.line_pixels();
            if (0 == line_px)
                return;

            //Returns the text position of each line that currently displays on screen.
            auto text_pos = textbox.text_position();

            //Textbox has supported smooth scrolling since 1.5. Therefore it would only render
            //the lower part of top displayed line if it is scrolled less than a line height.
            //
            //tbox.content_origin().y % line_px, get the height which is scrolled less than
            //a line height.
            int top = textbox.text_area().y - textbox.content_origin().y % line_px;
            int right = static_cast<int>(graph.width()) - 20;

            for (auto & pos : text_pos)
            {
                auto line_num = std::to_wstring(pos.y + 1);
                auto pixels = graph.text_extent_size(line_num).width;

                //Check if the panel widget is not enough room to display a line number
                if (pixels + 5 > graph.width())
                {
                    //Change the weight of 'line' field.
                    //layout.modify("lines", ("weight=" + std::to_string(pixels + 10)).c_str());
                    //layout.collocate();
                    //return;
                }

                //Draw the line number
                graph.string({ right - static_cast<int>(pixels), top }, line_num);

                top += line_px;
            }
        });
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
        elements_->tabs.toolbox(nana::tabbar<int>::kits::close, true);
        elements_->tabs.toolbox(nana::tabbar<int>::kits::list, true);
        elements_->tabs.toolbox(nana::tabbar<int>::kits::scroll, true);

        // Textbox setup
        setEditorEnabled(false);
        setHighlighting({DefaultHighlighting::CppDefault});
        elements_->textbox.typeface({"Consolas", 12});
        elements_->textbox.indention(true);
    }
//---------------------------------------------------------------------------------------------------------------------
    Editor::~Editor() = default;
//---------------------------------------------------------------------------------------------------------------------
    void Editor::setHighlighting(Highlighting const& highlight)
    {
        for (auto const& i : highlight.colors.keywordToColor)
        {
            elements_->textbox.set_highlight(i.first, i.second.first, i.second.second);
        }

        applyCppScheme(elements_->textbox);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::save()
    {
        elements_->fileStore.selected().data(elements_->textbox.caption());
        elements_->fileStore.selected().synchronizeToDisk();
        elements_->tabs.text(elements_->fileStore.index(), elements_->fileStore.selected().displayName());
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::saveAll()
    {
        elements_->fileStore.synchronizeAllToDisk();
        for (std::size_t i = 0; i != elements_->tabs.length(); ++i)
            elements_->tabs.text(i, elements_->fileStore[i].displayName());
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::loadFile(path const& projectRoot, path const& relativeToRootName)
    {
        auto& store = elements_->fileStore;

        if (!store.empty())
            setText(elements_->fileStore.selected().data());

        auto oldSize = store.size();
        auto newIndex = store.add(projectRoot, relativeToRootName);
        if (newIndex == -1)
            return;

        // set editor text
        auto& selected = store.selected();

        // store did not grow
        if (store.size() != oldSize)
            elements_->tabs.append(selected.displayName(), nullptr, selected.file().string());

        setEditorEnabled(true);
        tabSynchronize();

        elements_->textbox.colored_area_access();

        //elements_->textbox.load(filename.string());
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::registerTabEvents()
    {
        elements_->tabs.events().activated([this](auto const&)
        {
            elements_->fileStore.select(elements_->tabs.activated());
            setText(elements_->fileStore.selected().data());
        });

        elements_->tabs.events().removed([this](auto const& removed) mutable
        {
            bool doRemove = elements_->fileStore.remove(removed.value);
            const_cast <std::decay_t<decltype(removed)>&> (removed).remove = doRemove;

            if (elements_->fileStore.empty())
                setEditorEnabled(false);
            setText("");
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::setEditorEnabled(bool enabled)
    {
        elements_->textbox.enabled(enabled);
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
        elements_->layout["lines"] << elements_->lineNumbers;
        elements_->layout["tabbar"] << elements_->tabs;
        elements_->layout["editor"] << elements_->textbox;
        elements_->layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::focusTextbox()
    {
        elements_->textbox.focus();
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

        elements_->textbox.events().key_release([this](auto const& kb)
        {
            onTextboxKeyRelease(kb);
        });

        elements_->textbox.events().text_exposed([this]
        {
            nana::API::refresh_window(elements_->lineNumbers);
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void Editor::onTextboxKeyRelease(nana::arg_keyboard const& kbEvent)
    {
        // Keyboard
        if (kbEvent.key == 'S' && kbEvent.ctrl && !kbEvent.shift)
            save();
        if (kbEvent.key == 'S' && kbEvent.ctrl && kbEvent.shift)
            saveAll();

        // Automatic Whitespace fill in front
        #if 0
        if (kbEvent.key == nana::keyboard::enter)
        {
            auto yPrevious = elements_->textbox.caret_pos().y - 1;
            if (yPrevious >= 0)
            {
                std::string line;
                elements_->textbox.getline(yPrevious, line);
                std::string wspace;
                for (std::size_t i = 0; i != line.size(); ++i)
                {
                    if (std::isspace(line[i]))
                        wspace += line[i];
                    else
                        break;
                }
                elements_->textbox.append(wspace, true /*at caret*/);
            }
            return;
        }
        #endif
    }
//#####################################################################################################################
}
