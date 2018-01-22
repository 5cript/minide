#include "tool_options.hpp"

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/form.hpp>

namespace MinIDE
{
//#####################################################################################################################
    struct ToolOptionsImpl
    {
        ToolOptionsImpl(GlobalPersistence* settings);

        // Form
        nana::form form;
        GlobalPersistence* settings;
    };
//---------------------------------------------------------------------------------------------------------------------
    ToolOptionsImpl::ToolOptionsImpl(GlobalPersistence* settings)
        : form{nana::API::make_center(600,400)}
        , settings{settings}
    {
    }
//#####################################################################################################################
    ToolOptions::ToolOptions(GlobalPersistence* settings)
        : elements_{new ToolOptionsImpl(settings)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void ToolOptions::show()
    {
        elements_->form.show();
        elements_->form.wait_for_this();
    }
//---------------------------------------------------------------------------------------------------------------------
    ToolOptions::~ToolOptions() = default;
//#####################################################################################################################
}
