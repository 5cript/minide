#include "show_message.hpp"

#include <nana/gui.hpp>

namespace MinIDE
{
//#####################################################################################################################
    void showMessage(std::string const& message)
    {
        nana::msgbox mb(nullptr, "Message");
        mb.icon(mb.icon_question);
        mb << message;
        mb.show();
    }
//---------------------------------------------------------------------------------------------------------------------
    void showError(std::string const& error)
    {
        nana::msgbox mb(nullptr, "Error");
        mb.icon(mb.icon_question);
        mb << error;
        mb.show();
    }
//#####################################################################################################################
}
