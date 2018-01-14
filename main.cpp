#include "main_window.hpp"

#include <iostream>
#include <nana/gui.hpp>

int main()
{
    using namespace MinIDE;

    MainWindow window;
    window.show();
    nana::exec();
    return 0;
}
