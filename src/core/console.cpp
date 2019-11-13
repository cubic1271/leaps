#include "core/console.h"
#include "core/resource.h"
#include "core/state.h"

#define TWILIGHT_CONSOLE_ICON  "console.png"
#define TWILIGHT_CONSOLE_FONT  "robotomono.ttf"
#define TWILIGHT_CONSOLE_SIZE   (12)
#define TWILIGHT_CONSOLE_NROWS  (4)
#define TWILIGHT_CONSOLE_NCOLS  (80)

twilight::Console* _console = nullptr;

twilight::Console* twilight::Console::instance() {
    if(!_console) {
        _console = new Console;
    }
    return _console;
}

void twilight::Console::init() {
    ResourceManager* resource = ResourceManager::instance();

    icon = resource->getImage(TWILIGHT_CONSOLE_ICON);
    minimized = true;
    show = true;
    rows = TWILIGHT_CONSOLE_NROWS;
    cols = TWILIGHT_CONSOLE_NCOLS;
    console = new S2D_Text* [rows];
    text = new std::string[rows];
    for(int i = 0; i < rows; ++i) {
        console[i] = resource->getText(TWILIGHT_CONSOLE_FONT, TWILIGHT_CONSOLE_SIZE);
        S2D_SetText(console[i], "> TEST %d", i);
        lineheight = console[i]->height;
    }
}

void twilight::Console::inputMouse(S2D_Event* event) {
    WorldProjection* proj = WorldProjection::instance();
    b2Vec2 resolution = proj->getResolution();
    if(event->type == S2D_MOUSE_LEFT) {
        if(event->x > resolution.x - icon->width - 5 && event->y > resolution.y - icon->height - 5) {
            minimized = !minimized;
        }
    }
}

void twilight::Console::render() {
    WorldProjection* proj = WorldProjection::instance();
    b2Vec2 resolution = proj->getResolution();
    
    icon->x = resolution.x - icon->width - 5;
    icon->y = resolution.y - icon->height - 5;
    S2D_DrawImage(icon);

    if(!minimized) {
        S2D_DrawLine(
            0, resolution.y, 
            resolution.x, resolution.y, 
            lineheight * 8 + 40, 
            0.0, 0.0, 0.0, 0.7,
            0.0, 0.0, 0.0, 0.7,
            0.0, 0.0, 0.0, 0.7,
            0.0, 0.0, 0.0, 0.7);
        S2D_DrawLine(
            0, resolution.y - (lineheight * 4 + 22), 
            resolution.x, resolution.y - (lineheight * 4 + 22), 
            4, 
            1.0, 1.0, 1.0, 1.0,
            0.8, 0.8, 0.8, 1.0,
            1.0, 1.0, 1.0, 1.0,
            0.8, 0.8, 0.8, 1.0);
        for(int i = 0; i < rows; ++i) {
            console[i]->x = 10;
            console[i]->y = resolution.y - (lineheight * (4 - i)) - 10;
            S2D_DrawText(console[i]);
        }
    }
}

void twilight::Console::write(std::string str) {
    for(int i = 0; i < rows - 1; ++i) {
        text[i] = text[i + 1];
    }
    text[rows - 1] = std::string("> ") + str;
    for(int i = 0; i < rows; ++i) {
        S2D_SetText(console[i], text[i].c_str());
    }
}
