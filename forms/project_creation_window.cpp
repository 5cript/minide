#include "project_creation_window.hpp"

#include "../resources.hpp"

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>

namespace MinIDE
{
//#####################################################################################################################
    struct ProjectCreationWindowImpl
    {
        ProjectCreationWindowImpl(nana::window owner);

        nana::form form;
        nana::place layout;

        nana::label folderToCreateInLbl;
        nana::textbox folderToCreateIn;

        nana::label projectFolderNameLbl;
        nana::textbox projectFolderName;

        nana::label resultingPathLbl;
        nana::textbox resultingPath;
    };
//---------------------------------------------------------------------------------------------------------------------
    ProjectCreationWindowImpl::ProjectCreationWindowImpl(nana::window owner)
        : form{owner}
        , layout{form}
        , folderToCreateInLbl{form, "Folder to create folder for project in"}
        , folderToCreateIn{form}
        , projectFolderNameLbl{form, "Folder name"}
        , projectFolderName{form}
        , resultingPathLbl{form, "Resulting path"}
        , resultingPath{form}
    {

    }
//#####################################################################################################################
    ProjectCreationWindow::ProjectCreationWindow(
        nana::window owner
    )
        : impl_{new ProjectCreationWindowImpl(owner)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    ProjectCreationWindow::~ProjectCreationWindow() = default;
//---------------------------------------------------------------------------------------------------------------------
    void ProjectCreationWindow::show()
    {
        nana::API::modal_window(impl_->form);
        impl_->form.show();
    }
//---------------------------------------------------------------------------------------------------------------------
    void ProjectCreationWindow::setLayout()
    {
        auto& layout = impl_->layout;

        layout.field("FolderToCreateInLabel") << impl_->folderToCreateInLbl;
        layout.field("FolderToCreateIn") << impl_->folderToCreateIn;
        layout.field("ProjectFolderNameLabel") << impl_->projectFolderNameLbl;
        layout.field("ProjectFolderName") << impl_->projectFolderName;
        layout.field("ResultingPathLbl") << impl_->resultingPathLbl;
        layout.field("ResultingPath") << impl_->resultingPath;

        layout.div(loadResource("layouts/debugger_options.layout"));
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void ProjectCreationWindow::setupEvents()
    {

    }
//#####################################################################################################################
}
