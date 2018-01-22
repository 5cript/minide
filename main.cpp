#include "forms/main_window.hpp"
#include "global_settings/global_persistence.hpp"

#include <iostream>
#include <nana/gui.hpp>

int main()
{
    using namespace MinIDE;
    GlobalPersistence globalSettings;
    globalSettings.load();

    MainWindow window{&globalSettings};
    window.show();
    nana::exec();
    return 0;
}
