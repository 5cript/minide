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
        BuildAndRun
    };

    template <typename T>
    void populateToolbar(T& toolbar)
    {
        toolbar.append("Save File (Ctrl + S)", nana::paint::image{"resources/toolbar/save.png"}).enable(false);
        toolbar.append("Save All Files (Ctrl + Shift + S)", nana::paint::image{"resources/toolbar/save_all.png"}).enable(false);
        toolbar.separate();
        toolbar.append("Run CMake", nana::paint::image{"resources/toolbar/cmake.png"}).enable(false);
        toolbar.append("CMake + Make", nana::paint::image{"resources/toolbar/build.png"}).enable(false);
        toolbar.append("Run Program", nana::paint::image{"resources/toolbar/run.png"}).enable(false);
        toolbar.append("Build and Run (F5)", nana::paint::image{"resources/toolbar/build_run.png"}).enable(false);
    }

    template <auto elem, typename T>
    void activate(T& toolbar)
    {
        toolbar.enable(static_cast <int> (elem), true);
    }
}
