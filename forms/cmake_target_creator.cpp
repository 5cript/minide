#include "cmake_target_creator.hpp"
#include "../workspace/project_file/cmake_build_profile.hpp"

#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/checkbox.hpp>

namespace MinIDE
{
//#####################################################################################################################
    struct CMakeTargetCreatorImpl
    {
        CMakeTargetCreatorImpl(nana::window owner, GlobalPersistence* settings);

        nana::form form;
        GlobalPersistence* settings;

        // Widgets
        nana::label nameLabel;
        nana::textbox nameBox;
        nana::label envLabel;
        nana::combox envSelector;
        nana::label toolLabel;
        nana::combox toolSelector;
        nana::label outputLabel;
        nana::textbox outputPath;
        nana::label execLabel;
        nana::textbox execBox;
        nana::checkbox isDebugable;
        nana::label cmakeOptions;
        nana::checkbox isRelativeOutput;
        nana::label relativeOutputLabel;
        nana::textbox cmakeBox;
        nana::button okBtn;
        nana::button cancelBtn;

        // Layout
        nana::place layout;

        // Other
        bool clickedSave;

        // Target
        ProjectPersistence::CMakeBuildProfile target;
    };
//---------------------------------------------------------------------------------------------------------------------
    CMakeTargetCreatorImpl::CMakeTargetCreatorImpl(nana::window owner, GlobalPersistence* settings)
        : form{owner, nana::API::make_center(owner, 500, 240)}
        , nameLabel{form, "Name: "}
        , nameBox{form, "NewTarget"}
        , envLabel{form, "Environment: "}
        , envSelector{form}
        , toolLabel{form, "Tool Profile: "}
        , toolSelector{form}
        , outputLabel{form, "Build Directory: "}
        , outputPath{form}
        , execLabel{form, "Executable Name: "}
        , execBox{form}
        , isDebugable{form, "Can attach debugger"}
        , cmakeOptions{form, "CMake Options: "}
        , isRelativeOutput{form}
        , relativeOutputLabel{form, "Output Path is relative to project: "}
        , cmakeBox{form}
        , okBtn{form, "Save"}
        , cancelBtn{form, "Cancel"}
        , layout{form}
        , clickedSave{false}
        , target{}
    {
        nameBox.multi_lines(false);
        outputPath.multi_lines(false);
        execBox.multi_lines(false);
    }
//#####################################################################################################################
    CMakeTargetCreator::CMakeTargetCreator(nana::window owner, GlobalPersistence* settings)
        : elements_{new CMakeTargetCreatorImpl(owner, settings)}
    {
        setupLayout();
        setupEvents();
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeTargetCreator::setupEditorMasks()
    {
        auto noSpace = [](auto character) {
            if (std::isspace(character))
                return false;
            return true;
        };

        elements_->outputPath.set_accept(noSpace);
        elements_->execBox.set_accept(noSpace);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string CMakeTargetCreator::name() const
    {
        return elements_->target.name;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string CMakeTargetCreator::environment() const
    {
        return elements_->target.environment;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string CMakeTargetCreator::tooling() const
    {
        return elements_->target.toolProfile;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string CMakeTargetCreator::outputPath() const
    {
        return elements_->target.outputPath;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool CMakeTargetCreator::outputIsRelative() const
    {
        return elements_->target.outputIsRelative;
    }
//---------------------------------------------------------------------------------------------------------------------
    boost::optional <std::string> CMakeTargetCreator::executableName() const
    {
        return elements_->target.executable;
    }
//---------------------------------------------------------------------------------------------------------------------
    boost::optional <std::string> CMakeTargetCreator::cmakeOptions() const
    {
        return elements_->target.cmakeOptions;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool CMakeTargetCreator::isDebugable() const
    {
        return elements_->target.isDebugable;
    }
//---------------------------------------------------------------------------------------------------------------------
    CMakeTargetCreator::~CMakeTargetCreator() = default;
//---------------------------------------------------------------------------------------------------------------------
    void CMakeTargetCreator::setupEvents()
    {
        elements_->okBtn.events().click([this](auto const& event)
        {
            saveState();
            elements_->clickedSave = true;
            elements_->form.close();
        });
        elements_->cancelBtn.events().click([this](auto const& event)
        {
            elements_->clickedSave = false;
            elements_->form.close();
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeTargetCreator::saveState()
    {
        auto& target = elements_->target;

        target.name = elements_->nameBox.caption();
        target.outputPath = elements_->outputPath.caption();
        target.environment = elements_->envSelector.caption();
        target.toolProfile = elements_->toolSelector.caption();
        target.outputIsRelative = elements_->isRelativeOutput.checked();

        if (!elements_->execBox.empty())
            target.executable = elements_->execBox.caption();
        target.isDebugable = elements_->isDebugable.checked();
        if (!elements_->cmakeBox.empty())
            target.cmakeOptions = elements_->cmakeBox.caption();
    }
//---------------------------------------------------------------------------------------------------------------------
    bool CMakeTargetCreator::clickedSave()
    {
        return elements_->clickedSave;
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeTargetCreator::setup(
        std::vector <std::string> const& environments,
        std::vector <std::string> const& tools
    )
    {
        for (auto const& tool : tools)
            elements_->toolSelector.push_back(tool);
        for (auto const& env : environments)
            elements_->envSelector.push_back(env);
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeTargetCreator::setupLayout()
    {
        auto& layout = elements_->layout;

        layout.field("NameLabel") << elements_->nameLabel;
        layout.field("NameBox") << elements_->nameBox;
        layout.field("EnvLabel") << elements_->envLabel;
        layout.field("EnvSelector") << elements_->envSelector;
        layout.field("ToolLabel") << elements_->toolLabel;
        layout.field("ToolSelector") << elements_->toolSelector;
        layout.field("OutputPath") << elements_->outputLabel;
        layout.field("OutputBox") << elements_->outputPath;
        layout.field("OutputRelativeLabel") << elements_->relativeOutputLabel;
        layout.field("OutputRelativeBox") << elements_->isRelativeOutput;
        layout.field("ExecLabel") << elements_->execLabel;
        layout.field("ExecBox") << elements_->execBox;
        layout.field("CMakeOptions") << elements_->cmakeOptions;
        layout.field("CMakeBox") << elements_->cmakeBox;
        layout.field("IsDebugableChecker") << elements_->isDebugable;
        layout.field("Ok") << elements_->okBtn;
        layout.field("Cancel") << elements_->cancelBtn;

        layout.div(layoutString);
        layout.collocate();
    }
//---------------------------------------------------------------------------------------------------------------------
    void CMakeTargetCreator::show()
    {
        nana::API::modal_window(elements_->form);
        elements_->form.show();
    }
//#####################################################################################################################
}
