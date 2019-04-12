#pragma once

#include <nana/gui/widgets/form.hpp>

#include "../../filesystem.hpp"

#include <memory>
#include <vector>
#include <optional>

namespace MinIDE
{
    struct LocationSelectImpl;

    class LocationSelect
    {
    public:
        /**
         *  @param baseDirectory the directory to start in.
         *  @param directory Select directory instead of file.
         */
        LocationSelect(nana::window owner, std::string const& baseDirectory, bool directory);
        ~LocationSelect();

        /**
         *  Show dialog and optionally return a path to a file/directory.
         *  std::nullopt -> means cancel.
         */
        std::optional <path> show();

    private:
        void setupLayout();
        void setupEvents();

    private:
        std::unique_ptr <LocationSelectImpl> impl_;
    };
}
