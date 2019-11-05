#include "core/twilight.h"

#include <iostream>
#include <cassert>
#include <chrono>

#include "core/state.h"
#include "core/animation.h"
#include "core/resource.h"
#include "core/util.h"
#include "core/particle.h"
#include "core/config.h"
#include "leaps/state.h"

#define TWILIGHT_APP_VERSION        "v0.01"
#define TWILIGHT_TITLE_BUFSZ        (2048)

using namespace twilight;

static twilight::Timer _timer;
static double _accumulator;

static void _render() {
    ParticleManager* particle = ParticleManager::instance();
    particle->render();
}

static void _update() {
    ParticleManager* particle = ParticleManager::instance();
    StateManager* state = StateManager::instance();

    double dt = _timer.elapsed();
    _accumulator += dt;
    while(_accumulator > TWILIGHT_UPDATE_DELTA) {
        particle->update(dt);
        _accumulator -= TWILIGHT_UPDATE_DELTA;
    }
    _timer.update();
}

static void _onkey(S2D_Event key) {
    StateManager* state = StateManager::instance();
    state->inputKey(&key);
}

static void _onmouse(S2D_Event mouse) {
    StateManager* state = StateManager::instance();
    state->inputMouse(&mouse);
}

static void _oncontroller(S2D_Event controller) {
    StateManager* state = StateManager::instance();
    state->inputController(&controller);
}

int main(int argc, char* argv[]) {
    char* tbuf = new char[TWILIGHT_TITLE_BUFSZ];
    _accumulator = 0.0;
    _timer.start();
    ResourceManager* resource = ResourceManager::instance();
    resource->init("resource");
    ConfigManager* config = ConfigManager::instance();
    config->setPath(resource->getBase() + "/config.xml");
    assert(config != nullptr);
    int width = config->getSettingAsInteger("ScreenWidth");
    int height = config->getSettingAsInteger("ScreenHeight");
    double physWidth = config->getSettingAsFloat("WorldSizeX");
    double physHeight = config->getSettingAsFloat("WorldSizeY");
    printf("Building window of size: %d x %d\n", width, height);
    printf("Fullscreen: %d\n", config->getSettingAsInteger("FullScreen"));
    int flags = (config->getSettingAsBoolean("FullScreen") ? S2D_FULLSCREEN : 0);
    LeapsStateInit();
    StateManager* state = StateManager::instance();
    WorldProjection* projection = WorldProjection::instance();
    projection->set(b2Vec2(width, height), b2Vec2(physWidth, physHeight));

    snprintf(tbuf, TWILIGHT_TITLE_BUFSZ, "LEAPS / %s / %s", TWILIGHT_APP_VERSION, __DATE__);
    S2D_Window* window = S2D_CreateWindow(tbuf, width, height, _update, _render, flags);
    window->on_key = _onkey;
    window->on_mouse = _onmouse;
    window->on_controller = _oncontroller;

    StateEntry* start = state->getState(LEAPS_TITLE_NAME);
    assert(nullptr != start);
    state->startTransition(start);
    S2D_Show(window);
    delete[] tbuf;
    return 0;
}
