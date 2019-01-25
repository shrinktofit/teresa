
#include "app.h"
#include <iostream>

namespace teresa
{
    webgl_canvas::webgl_canvas()
        :_displayWindow(std::make_unique<glfw_window>(640, 480, "Display screen"))
    {

    }

    void webgl_canvas::flush()
    {
        // std::cout << _flushCount++ << "\n";
        _displayWindow->react();
    }
}
