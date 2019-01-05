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

using twilight::Timer;
using twilight::StateManager;
using twilight::ResourceManager;
using twilight::AnimationManager;
using twilight::ParticleManager;
using twilight::ConfigManager;

static twilight::Timer _timer;
static double _accumulator;

static void _render() {

}

static void _update() {
    ParticleManager* particle = ParticleManager::instance();
    double dt = _timer.elapsed();
    _accumulator += dt;
    while(_accumulator > TWILIGHT_UPDATE_DELTA) {
        particle->update(dt);
        _accumulator -= TWILIGHT_UPDATE_DELTA;
    }
    _timer.update();
}

int main(int argc, char* argv[]) {
    _accumulator = 0.0;
    _timer.start();
    ResourceManager* resource = ResourceManager::instance();
    resource->init("resource");
    ConfigManager* config = ConfigManager::instance();
    config->setPath(resource->getBase() + "/config.xml");
    assert(config != nullptr);
    int width = config->getSettingAsInteger("ScreenWidth");
    int height = config->getSettingAsInteger("ScreenHeight");
    printf("Building window of size: %d x %d\n", width, height);
    printf("Fullscreen: %d\n", config->getSettingAsInteger("FullScreen"));
    int flags = (config->getSettingAsBoolean("FullScreen") ? S2D_FULLSCREEN : 0);
    S2D_Window* window = S2D_CreateWindow("TwilightEngine v0.25", width, height, _update, _render, flags);
    S2D_Show(window);
    return 0;
}
