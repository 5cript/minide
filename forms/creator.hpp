#pragma once

#include "creator/createable.hpp"

#include <nana/gui/widgets/form.hpp>

#include <memory>
#include <vector>
#include <optional>

namespace MinIDE
{
    struct CreatorImpl;

    class Creator
    {
    public:
        Creator(nana::window owner, std::unordered_map <std::string, std::vector <Creatable>> creatables);
        Creator(nana::window owner, std::string const& creatableList);
        ~Creator();
        std::optional <Creatable> show();

    private:
        void setupLayout();
        void setupEvents();

    private:
        std::unique_ptr <CreatorImpl> impl_;
    };
}
