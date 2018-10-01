#pragma once

namespace MinIDE
{
    enum class ToolbarElement
    {
        Save = 0,
        SaveAll,
        _Splitter_1,
        CMake,
        Build,
        Run,
        BuildAndRun,
        Cancel,
        _Splitter_2,
        DebugContinue,
        NextLine,
        StepInto,
        StepOut
    };

    template <typename T>
    void populateToolbar(T& toolbar)
    {
        toolbar.append("Save File (Ctrl + S)", nana::paint::image{"resources/images/toolbar/save.png"}).enable(false);
        toolbar.append("Save All Files (Ctrl + Shift + S)", nana::paint::image{"resources/images/toolbar/save_all.png"}).enable(false);
        toolbar.separate();
        toolbar.append("Run CMake", nana::paint::image{"resources/images/toolbar/cmake.png"}).enable(false);
        toolbar.append("CMake + Make", nana::paint::image{"resources/images/toolbar/build.png"}).enable(false);
        toolbar.append("Run Program", nana::paint::image{"resources/images/toolbar/run.png"}).enable(false);
        toolbar.append("Build and Run (F5)", nana::paint::image{"resources/images/toolbar/build_run.png"}).enable(false);
        toolbar.append("Kill Process", nana::paint::image{"resources/images/toolbar/red_x.png"}).enable(false);
        toolbar.separate();
        toolbar.append("Debug/Continue", nana::paint::image{"resources/images/toolbar/debug.png"}).enable(false);
        toolbar.append("Next Line", nana::paint::image{"resources/images/toolbar/next_line.png"}).enable(false);
        toolbar.append("Step Into", nana::paint::image{"resources/images/toolbar/step_into.png"}).enable(false);
        toolbar.append("Step Out", nana::paint::image{"resources/images/toolbar/step_out.png"}).enable(false);
    }

    template <auto elem, typename T>
    void activate(T& toolbar)
    {
        toolbar.enable(static_cast <int> (elem), true);
    }

    template <auto elem, typename T>
    void deactivate(T& toolbar)
    {
        toolbar.enable(static_cast <int> (elem), false);
    }
}
