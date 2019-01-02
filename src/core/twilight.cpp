#include "core/twilight.h"

#include <iostream>
#include <cassert>
#include <chrono>

#include "core/state.h"
#include "core/animation.h"
#include "core/resource.h"
#include "core/util.h"

using twilight::Timer;
using twilight::StateManager;
using twilight::ResourceManager;
using twilight::AnimationManager;

static twilight::Timer _timer;

static void _render() {

}

static void _update() {
    double dt = _timer.elapsed();
    _timer.update();
}

int main(int argc, char* argv[]) {
    _timer.start();
    StateManager* state = StateManager::instance();
    ResourceManager* resource = ResourceManager::instance();
    resource->init("resource");
    S2D_Window* window = S2D_CreateWindow("TwilightEngine v0.2", 1920, 1080, _update, _render, S2D_FULLSCREEN);
    S2D_Show(window);
    return 0;
}
