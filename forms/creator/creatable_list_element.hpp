#pragma once

#include "createable.hpp"

#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/label.hpp>

#include <iostream>

namespace MinIDE
{
    class CreatableListElement : public nana::listbox::inline_notifier_interface
    {
    public:
        explicit CreatableListElement(int imageLabelPadding = 5, int imageBorderPadding = 3)
            : indicator_{nullptr}
            , pos_{}
            , image_{}
            , name_{}
            , imageLabelPadding_{imageLabelPadding}
            , imageBorderPadding_{imageBorderPadding}
        {
        }

    private: // NVI
        void create(nana::window wd) override
        {
            image_.create(wd);
            name_.create(wd);
            image_.move(imageBorderPadding_, imageBorderPadding_);

            name_.caption("blalblalalala");

            auto regEvents = [this](auto& what)
            {
                what.events().click([this]
                {
                    indicator_->selected(pos_);
                });

                what.events().mouse_move([this]
                {
                    indicator_->hovered(pos_);
                });;
            };

            regEvents(image_);
            regEvents(name_);
        }

        void activate(inline_indicator& ind, index_type pos) override
        {
            indicator_ = &ind;
            pos_ = pos;
        }

        void resize(const nana::size& dimension) override
        {
            auto const& sz = dimension;

            image_.size(nana::size{sz.height - imageBorderPadding_ * 2, sz.height - imageBorderPadding_ * 2});
            name_.size(nana::size{sz.width - sz.height - imageBorderPadding_ * 2 - imageLabelPadding_, sz.height});

            //name_.move(sz.height + imageBorderPadding_ * 2 + imageLabelPadding_, sz.height);
            name_.move(image_.size().width + 2*imageBorderPadding_, imageBorderPadding_);

            // FIXME: static color.
            name_.bgcolor(nana::color{0,0,0,1.});
            image_.bgcolor(nana::color{0,0,0,1.});
            name_.transparent(true);
            image_.transparent(true);
        }

        bool whether_to_draw() const override
        {
            return false;
        }

        void notify_status(status_type, bool) override
        {
        }

        void set(const value_type& value) override
        {
            using namespace std::string_literals;

            creatable_ = Creatable::splitParse(value);

            if (creatable_.imageResource())
                //image_.caption("<image=\""s + creatable_.imageResource().value() + "\">");
                image_.load(nana::paint::image{creatable_.imageResource().value()});

            name_.caption(creatable_.name());
        }

    private:
        inline_indicator* indicator_;
        index_type pos_;
        Creatable creatable_;
        nana::picture image_;
        nana::label name_;
        int imageLabelPadding_;
        int imageBorderPadding_;
    };
}
