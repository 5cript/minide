#pragma once

#include <nana/gui/basis.hpp>

namespace MinIDE
{
    constexpr unsigned int bgGray = 0x303030;

    struct ProjectTreeTheme
    {
        nana::color highlightGradient1 = nana::color_rgb(0x0);
        nana::color highlightGradient2 = nana::color_rgb(bgGray);
        nana::color selectGradient1 = nana::color_rgb(0x0);
        nana::color selectGradient2 = nana::color_rgb(bgGray);
        nana::color shGradient1 = nana::color_rgb(0x0);
        nana::color shGradient2 = nana::color_rgb(bgGray);
        nana::color text = nana::color_rgb(0xEEEEEE);
    };

    struct Theme
    {
        nana::color largeBackgrounds = nana::color_rgb(bgGray);
        nana::color textForegrounds = nana::color_rgb(0xEEEEEE);

        ProjectTreeTheme projTree;
    };
}
