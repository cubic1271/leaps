#ifndef _TWILIGHT_CONSOLE_H
#define _TWILIGHT_CONSOLE_H

#include "simple2d/simple2d.h"

#include <string>
#include <vector>

namespace twilight {
    class Console {
    public:
        static Console*  instance();
        void             init();
        void             render();
        void             write(std::string str);
        void             toggle();
        void             inputMouse(S2D_Event* event);

    protected:
        S2D_Image*   icon;
        bool         minimized;
        bool         show;
        S2D_Text**   console;
        std::string* text;
        int          rows;
        int          cols;
        int          lineheight;
    };
}

#endif
