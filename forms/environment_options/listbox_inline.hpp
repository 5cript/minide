#pragma once

#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>

namespace MinIDE
{
    class EditableListElement : public nana::listbox::inline_notifier_interface
    {
    public:
        EditableListElement(bool* dirty)
            : indicator_{nullptr}
            , pos_{}
            , txt_{}
            , dirty_{dirty}
            , fresh_{false}
        {}

    private:
        //Creates inline widget
        //listbox calls this method to create the widget
        //The position and size of widget can be ignored in this process
        void create(nana::window wd) override
        {
            //Create listbox
            txt_.create(wd);
            txt_.events().click([this]
            {
                //Select the item when clicks the textbox
                indicator_->selected(pos_);
            });

            txt_.events().mouse_move([this]
            {
                //Highlight the item when hovers the textbox
                indicator_->hovered(pos_);
            });

            txt_.events().text_changed([this](const auto& arg)
            {
                std::cout << txt_.caption() << "\n";
                indicator_->modify(pos_, txt_.caption());
                if (!fresh_)
                    *dirty_ = true;
                fresh_ = false;
            });

            txt_.borderless(true);
            txt_.multi_lines(false);
        }

        //Activates the inline widget, bound to a certain item of the listbox
        //The inline_indicator is an object to operate the listbox item,
        //pos is an index object refers to the item of listbox
        void activate(inline_indicator& ind, index_type pos) override
        {
            indicator_ = &ind;
            pos_ = pos;
        }

            //Notify the item status
        //This method is introduced since 1.4.1
        void notify_status(status_type status, bool status_on) /*override*/
        {
            switch(status)
            {
            default: break;
            case status_type::selecting:
                //If status_on is true, the item is selecting.
                //If status on is false, the item is unselecting
                break;
            case status_type::checking:
                //If status_on is true, the item is checking
                //If status_on is false, the item is unchecking
                break;
            }
		}

		//Sets the inline widget size
        //dimension represents the max size can be set
        //The coordinate of inline widget is a logical coordinate to the sub item of listbox
        void resize(const nana::size& dimension) override
        {
            auto sz = dimension;
            txt_.size(sz);

            //nana::rectangle r(sz.width + 5, 0, 45, sz.height);
            //btn_.move(r);
        }

        //Sets the value of inline widget with the value of the sub item
        void set(const value_type& value) override
        {
            if (txt_.caption() == value)
                return;
            fresh_ = true;
            auto caret = txt_.caret_pos();
            txt_.caption(value);
            txt_.caret_pos(caret);
        }

        //Determines whether to draw the value of sub item
        //e.g, when the inline widgets covers the whole background of the sub item,
        //it should return false to avoid listbox useless drawing
        bool whether_to_draw() const override
        {
            return false;
        }

    private:
        inline_indicator* indicator_;
        index_type pos_;
        nana::textbox txt_;
        bool* dirty_;
        bool fresh_;
    };
}
