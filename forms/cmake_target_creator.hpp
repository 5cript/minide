#pragma once

#include "../global_settings/global_persistence.hpp"

#include <nana/gui/widgets/form.hpp>

#include <boost/optional.hpp>
#include <memory>
#include <vector>
#include <string>

namespace MinIDE
{
    struct CMakeTargetCreatorImpl;

    class CMakeTargetCreator
    {
    public:
        constexpr static const char* layoutString =
            #include "layouts/CMakeTargetCreator.layout"
        ;

    public:
        CMakeTargetCreator(nana::window owner, GlobalPersistence* settings);
        ~CMakeTargetCreator();

        void setup(
            std::vector <std::string> const& environments,
            std::vector <std::string> const& tools
        );
        void show();
        bool clickedSave();

        std::string name() const;
        std::string environment() const;
        std::string tooling() const;
        std::string outputPath() const;
        boost::optional <std::string> executableName() const;
        boost::optional <std::string> cmakeOptions() const;
        bool isDebugable() const;

    private:
        void setupLayout();
        void setupEvents();
        void setupEditorMasks();
        void saveState();

    private:
        std::unique_ptr <CMakeTargetCreatorImpl> elements_;
    };
}
