#include "location_select.hpp"

#include "../../resources.hpp"

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>

namespace MinIDE
{
//#####################################################################################################################
    struct LocationSelectImpl
    {
        LocationSelectImpl(nana::window owner);

        // Form
        nana::form form;

        // Widgets
        nana::button ok;
        nana::button cancel;
        nana::textbox location;
        nana::button dialogPopper;

        // Layout
        nana::place layout;
    };
//---------------------------------------------------------------------------------------------------------------------
    LocationSelectImpl::LocationSelectImpl(nana::window owner)
        : form{owner, nana::API::make_center(owner, 800, 200)}
        , ok{form, "Ok"}
        , cancel{form, "Cancel"}
        , location{form}
        , dialogPopper{form}
        , layout{form}
    {

    }
//#####################################################################################################################
    LocationSelect::LocationSelect(nana::window owner, std::string const& baseDirectory, bool directory)
        : impl_{new LocationSelectImpl(owner)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    LocationSelect::~LocationSelect() = default;
//---------------------------------------------------------------------------------------------------------------------
    std::optional <path> LocationSelect::show()
    {
        return std::nullopt;
    }
//---------------------------------------------------------------------------------------------------------------------
    void LocationSelect::setupLayout()
    {
        auto& layout = impl_->layout;

        layout.field("DialogPopper") << impl_->dialogPopper;
        layout.field("OkButton") << impl_->ok;
        layout.field("CancelButton") << impl_->cancel;
        layout.field("LocationBox") << impl_->location;

        layout.div(loadResource("layouts/location_select.layout"));
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void LocationSelect::setupEvents()
    {

    }
//#####################################################################################################################
}
