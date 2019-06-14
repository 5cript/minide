#include "creator.hpp"
#include "creator/creatable_list_element.hpp"

#include "../resources.hpp"

#include <SimpleJSON/parse/jsd_convenience.hpp>

#include <nana/gui/widgets/form.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>

namespace MinIDE
{
//#####################################################################################################################
    struct CreatorImpl
    {
        CreatorImpl(nana::window owner, std::vector <Creatable>&& creatables);

        std::vector <Creatable> creatables;

        // Form
        nana::form form;

        // Widgets
        nana::listbox selector;
        nana::button ok;
        nana::button cancel;
        nana::textbox description;

        // Layout
        nana::place layout;

        // Other
        std::optional <Creatable> selected;
    };
//---------------------------------------------------------------------------------------------------------------------
    CreatorImpl::CreatorImpl(nana::window owner, std::vector <Creatable>&& creatables)
        : creatables{std::move(creatables)}
        , form{owner, nana::API::make_center(owner, 600, 400)}
        , selector{form}
        , ok{form, "Ok"}
        , cancel{form, "Cancel"}
        , description{form}
        , layout{form}
        , selected{std::nullopt}
    {
        form.caption("Creator");

        selector.append_header("Type");
        selector.at(0).inline_factory(0, nana::pat::make_factory<CreatableListElement>());

        for (auto const& c : this->creatables)
        {
            selector.at(0).append(c.catenated());
        }

        description.line_wrapped(true);
        description.editable(false);
    }
//#####################################################################################################################
    Creator::Creator(nana::window owner, std::vector <Creatable> creatables)
        : impl_{new CreatorImpl(owner, std::move(creatables))}
    {
        setupLayout();
        setupEvents();
    }
//---------------------------------------------------------------------------------------------------------------------
    Creator::Creator(nana::window owner, std::string const& creatableList)
        : Creator(owner, [&creatableList](){
            auto creas = JSON::make_from_json <SerializableCreatables>(creatableList);
            std::vector <Creatable> creatables;
            for (auto& elem : creas.creatables)
            {
                elem.script = loadResource(elem.script);
                if (elem.image)
                    elem.image = resource(elem.image.value()).string();
                creatables.push_back(elem);
            }
            return creatables;
        }())
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    Creator::~Creator() = default;
//---------------------------------------------------------------------------------------------------------------------
    std::optional <Creatable> Creator::show()
    {
        nana::API::modal_window(impl_->form);
        impl_->form.show();
        return impl_->selected;
    }
//---------------------------------------------------------------------------------------------------------------------
    void Creator::setupLayout()
    {
        auto& layout = impl_->layout;

        layout.field("Selector") << impl_->selector;
        layout.field("OkButton") << impl_->ok;
        layout.field("CancelButton") << impl_->cancel;
        layout.field("Description") << impl_->description;

        layout.div(loadResource("layouts/creator_window.layout"));
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void Creator::setupEvents()
    {
        impl_->ok.events().click([this](auto const&) {
            if (impl_->selector.selected().empty())
            {
                impl_->selected = std::nullopt;
                impl_->form.close();
                return;
            }
            auto indexPair = impl_->selector.selected().front();
            if (indexPair.item != decltype(indexPair)::npos)
                impl_->selected = impl_->creatables[indexPair.item];
            else
                impl_->selected = std::nullopt;
            impl_->form.close();
        });

        impl_->cancel.events().click([this](auto const&) {
            impl_->selected = std::nullopt;
            impl_->form.close();
        });

        impl_->selector.events().selected([this](auto const& item){
            auto indexPair = item.item.pos();
            impl_->description.caption(impl_->creatables[indexPair.item].description());
        });
    }
//#####################################################################################################################
}
