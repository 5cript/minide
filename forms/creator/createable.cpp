#include "createable.hpp"
#include "../../scripting_engine/api/wizard.hpp"
#include "../../resources.hpp"

#include <nana/gui/widgets/form.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/paint/graphics.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui.hpp>

#include <boost/algorithm/string.hpp>
#include <fmt/core.h>

#include <optional>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#define WIZARD_PARAM_DECONSTRUCT auto& [id, prettyName, value, description, isOptional, orderHint]

namespace MinIDE
{
//#####################################################################################################################
    namespace
    {
        struct WizardState
        {
            MinIDE::Scripting::Api::Wizard const* wizard;
            MinIDE::Scripting::Api::Wizard::Parameters parameters;
            nana::group* group;
        };

        void constructParameterPanel(WizardState& state)
        {
            state.parameters = state.wizard->retrieveParameters();

            std::string layoutString = "<vertical <vertical margin=3 <weight=5>\n";

            int maxExtent = 0;
            for (auto const& [key, param] : state.parameters)
            {
                WIZARD_PARAM_DECONSTRUCT = param;

                nana::paint::graphics graph{nana::size{1,1}};
                graph.typeface(nana::paint::font("Tahoma", 11));
                nana::size ext = graph.text_extent_size(prettyName.c_str());

                maxExtent = std::max(static_cast <int> (ext.width), maxExtent);
            }

            std::vector <MinIDE::Scripting::Api::Wizard::Parameter*> orderedParams;
            for (auto& [key, param] : state.parameters)
            {
                orderedParams.push_back(&param);
            }
            std::sort(std::begin(orderedParams), std::end(orderedParams), [](auto const& lhs, auto const& rhs)
            {
                return lhs->orderHint < rhs->orderHint;
            });

            for (auto& param : orderedParams)
            {
                WIZARD_PARAM_DECONSTRUCT = *param;

                auto* lbl = state.group->create_child <nana::label>((id + "_LABEL").c_str());
                auto* box = state.group->create_child <nana::textbox>(id.c_str());

                box->caption(value);
                lbl->caption(prettyName);

                box->tip_string(description.c_str());
                box->events().text_changed([&value](auto const& tb)
                {
                    value = tb.widget.caption();
                });

                layoutString += fmt::format(R"(
                    <max=25
                        <weight=5>
                        <vertical max={1}
                            <weight=3>
                            <{0}_LABEL>
                        >
                        <{0}>
                    >
                    <weight=3>
                )", id, std::to_string(maxExtent + 10));
            }
            layoutString += R"(
                    >
                >
            )";

            state.group->div(layoutString.c_str());
            state.group->collocate();
        }
    }
//#####################################################################################################################
    Creatable::Creatable(std::string name, std::string description, Scripting::Script script, std::optional <std::string> imageResource)
        : name_{std::move(name)}
        , description_{std::move(description)}
        , script_{std::move(script)}
        , imageResource_{std::move(imageResource)}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    Creatable::Creatable(SerializableCreatable const& seriCreate)
        : name_{seriCreate.name}
        , description_{seriCreate.description}
        , script_{seriCreate.script}
        , imageResource_{seriCreate.image}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Creatable::description() const
    {
        return description_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Creatable::name() const
    {
        return name_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::optional <std::string> Creatable::imageResource() const
    {
        return imageResource_;
    }
//---------------------------------------------------------------------------------------------------------------------
    Scripting::Script const& Creatable::script() const
    {
        return script_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Creatable::catenated() const
    {
        // 0 = name
        // 1 = descr
        // 2 = image

        std::string cat;
        cat += name_ + ";";
        cat += description_ + ";";
        if (imageResource_)
            cat += imageResource_.value();

        return cat;
    }
//---------------------------------------------------------------------------------------------------------------------
    Creatable Creatable::splitParse(std::string const& str)
    {
        if (str.empty())
            return {};

        std::vector<std::string> strs;
        boost::split(strs, str, boost::is_any_of(";"));

        switch(strs.size())
        {
        case(0):
            return {};
        case(1):
            return {strs[0], strs[0], {}, std::nullopt};
        case(2):
            return {strs[0], strs[1], {}, std::nullopt};
        case(3):
            return {strs[0], strs[1], {}, strs[2]};
        default:
            return {};
        }

        return {};
    }
//---------------------------------------------------------------------------------------------------------------------
    std::optional <path> Creatable::startWizard(std::string const& initPath, std::string const& basePath)
    {
        using namespace Scripting::Api;

        WizardState state{};
        Wizard wizard{&script_};
        state.wizard = &wizard;

        auto type = wizard.retrieveType();
        if (type != "directory" && type != "file")
        {
            throw std::runtime_error ("unknown wizard type, expected 'directory' or 'file'");
        }

        nana::form inputForm{nana::API::make_center(700, 450)};
        nana::place layout{inputForm};
        nana::button ok{inputForm, "Ok"};
        nana::button cancel{inputForm, "Cancel"};
        nana::group scriptParameters{inputForm, "Wizard Parameters"};
        state.group = &scriptParameters;

        nana::label fullPath{inputForm, "Full Path: "};
        nana::textbox pathBox{inputForm};
        nana::button pathChooserOpen{inputForm, "..."};

        inputForm.caption("Wizard");

        constructParameterPanel(state);

        bool okayed{false};
        path root{};

        pathChooserOpen.events().click([&type, &pathBox, &initPath, &wizard, &inputForm, &okayed](auto const& btn)
        {
            if (type == "directory")
            {
                if (!initPath.empty())
                {
                    nana::folderbox picker{nullptr, initPath};

                    auto paths = picker.show();
                    if (!paths.empty())
                        pathBox.caption(paths.front().string());
                }
                else
                {
                    nana::folderbox picker;

                    auto paths = picker.show();
                    if (!paths.empty())
                        pathBox.caption(paths.front().string());
                }
            }
            else if (type == "file")
            {
                try
                {
                    nana::filebox picker{nullptr, false};
                    picker.allow_multi_select(false);

                    if (!initPath.empty())
                        picker.init_path(initPath);

                    picker.add_filter(wizard.getFilters());

                    auto paths = picker.show();
                    if (!paths.empty())
                        pathBox.caption(paths.front().string());
                }
                catch(std::exception const& exc)
                {
                    nana::msgbox mb(inputForm, "Error", nana::msgbox::ok);
                    mb << "Error in wizard script. Closing Wizard Dialog";
                    mb.show();
                    okayed = false;
                    inputForm.close();
                    return;
                }
            }
            //pathBox.caption()
        });

        ok.events().click([&okayed, &inputForm, &pathBox, &initPath, &root, &state, &basePath](auto const& btn)
        {
            // Check if set path is ok
            if (pathBox.caption().empty())
            {
                nana::msgbox mb(inputForm, "Error", nana::msgbox::ok);
                mb << "The path to create the project/file at may not be empty";
                mb.show();
                return;
            }

            // Check if there is an absolute path when the basePath is empty
            if (basePath.empty() && !path{pathBox.caption()}.is_absolute())
            {
                nana::msgbox mb(inputForm, "Error", nana::msgbox::ok);
                mb << "The path must be an absolute path, unless you got here from the project tree popup menu.";
                mb.show();
                return;
            }

            // Check for non optionals
            for ([[maybe_unused]] auto const& [key, param] : state.parameters)
            {
                if (!param.isOptional && param.defaultValue.empty())
                {
                    nana::msgbox mb(inputForm, "Error", nana::msgbox::ok);
                    mb << "The parameter '" << param.prettyName << "' must not be empty.\n";
                    mb << "Description of parameter: " << param.description;
                    mb.show();
                    return;
                }
            }

            if (!basePath.empty() && path{pathBox.caption()}.is_absolute())
                root = path{basePath} / pathBox.caption();
            else
                root = path{pathBox.caption()};

            bool canceled;
            if (filesystem::is_directory(root) && !filesystem::is_empty(root))
            {
                nana::msgbox mb(inputForm, "Warning", nana::msgbox::yes_no);
                mb.icon(mb.icon_question);
                mb << L"The directory is not empty, do you want to continue anyway?";
                canceled = (mb.show() != nana::msgbox::pick_yes);
            }

            if (!canceled)
            {
                okayed = true;
                inputForm.close();
            }
        });

        cancel.events().click([&okayed, &inputForm](auto const& btn)
        {
            okayed = false;
            inputForm.close();
        });

        inputForm.size({700u, static_cast <unsigned int> (state.parameters.size()) * 28 + 30 + 80});

        layout["OkButton"] << ok;
        layout["CancelButton"] << cancel;
        layout["ScriptParameters"] << scriptParameters;
        layout["FullPathLabel"] << fullPath;
        layout["PathBox"] << pathBox;
        layout["DialogOpener"] << pathChooserOpen;

        layout.div(loadResource("layouts/wizard_window.layout"));
        layout.collocate();
        nana::API::modal_window(inputForm);
        inputForm.show();

        // And now for the magic :D!

        if (!okayed)
            return std::nullopt;

        auto creas = wizard.runWizard(state.parameters);

        if (type == "directory")
        {
            // Validate all paths
            for (auto const& c : creas)
                (void)jail(root, c.name);

            for (auto const& c : creas)
            {
                auto path = root / jail(root, c.name);
                if (c.type == "directory")
                {
                    filesystem::create_directory(path);
                }
                else if (c.type == "file")
                {
                    if (filesystem::exists(path))
                    {
                        nana::msgbox mb(inputForm, "Warning", nana::msgbox::yes_no);
                        mb.icon(mb.icon_question);
                        mb << "The file " << path.string() << " already exists! Overwrite?";
                        if(mb.show() != nana::msgbox::pick_yes)
                            continue;
                    }
                    std::ofstream writer{path, std::ios_base::binary};
                    writer.write(c.content.c_str(), c.content.length());
                }
            }
        }
        else if (type == "file")
        {
            auto c = creas.front();

            auto path = root;
            if (filesystem::exists(path))
            {
                nana::msgbox mb(inputForm, "Warning", nana::msgbox::yes_no);
                mb.icon(mb.icon_question);
                mb << "The file " << path.string() << " already exists! Overwrite?";
                if(mb.show() != nana::msgbox::pick_yes)
                    return {root};
            }
            std::ofstream writer{path, std::ios_base::binary};
            writer.write(c.content.c_str(), c.content.length());
        }

        return {root};
    }
//#####################################################################################################################
}
