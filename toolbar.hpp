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
        toolbar.append("Save File (Ctrl + S)", nana::paint::image{"resources/toolbar/save.png"});
        toolbar.append("Save All Files (Ctrl + Shift + S)", nana::paint::image{"resources/toolbar/save_all.png"});
        toolbar.separate();
        toolbar.append("Run CMake", nana::paint::image{"resources/toolbar/cmake.png"});
        toolbar.append("CMake + Make", nana::paint::image{"resources/toolbar/build.png"});
        toolbar.append("Run Program", nana::paint::image{"resources/toolbar/run.png"});
        toolbar.append("Build and Run (F5)", nana::paint::image{"resources/toolbar/build_run.png"});
    }
}
