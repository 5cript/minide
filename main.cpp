#include "forms/main_window.hpp"
#include "global_settings/global_persistence.hpp"
#include "resources.hpp"

#include <iostream>
#include <nana/gui.hpp>

void initializePython(char** argv);

int main(int argc, char** argv)
{
    using namespace MinIDE;
    GlobalPersistence globalSettings;
    globalSettings.load();

    MainWindow window{&globalSettings};
    window.show();
    nana::exec();
    return 0;
}
