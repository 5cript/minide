#pragma once

#include <nana/gui/widgets/form.hpp>

namespace MinIDE
{
    struct ProjectCreationWindowImpl;

    class ProjectCreationWindow
    {
    public:
        ProjectCreationWindow(
            nana::window owner
        );

        ~ProjectCreationWindow();
        void show();

    private:
        void setLayout();
        void setupEvents();

    private:
        std::unique_ptr <ProjectCreationWindowImpl> impl_;
    };
}
