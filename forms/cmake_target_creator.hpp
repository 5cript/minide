#pragma once

#include "../global_settings/global_persistence.hpp"
#include "../workspace/project_file/cmake_build_profile.hpp"

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
        CMakeTargetCreator(nana::window owner, GlobalPersistence* settings);
        ~CMakeTargetCreator();

        void setup(
            std::vector <std::string> const& environments,
            std::vector <std::string> const& tools
        );
        void setup(
            std::vector <std::string> const& environments,
            std::vector <std::string> const& tools,
            ProjectPersistence::CMakeBuildProfile* target
        );
        void show();
        bool clickedSave();

        std::string name() const;
        std::string originalName() const;
        std::string environment() const;
        std::string tooling() const;
        std::string outputPath() const;
        bool outputIsRelative() const noexcept;
        std::optional <std::string> executableName() const;
        std::optional <std::string> cmakeOptions() const;
        bool isDebugable() const noexcept;
        std::optional <std::string> makeOptions() const;
        std::optional <std::string> arguments() const;
        std::optional <std::string> executionDirectory() const;
        bool executionDirectoryIsRelative() const noexcept;

        ProjectPersistence::CMakeBuildProfile getTarget() const;

    private:
        void setupLayout();
        void setupEvents();
        void setupEditorMasks();
        void saveState();

    private:
        std::unique_ptr <CMakeTargetCreatorImpl> elements_;
    };
}
