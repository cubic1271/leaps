#include <core/twilight.h>
#include <cmath>
#include <simple2d/simple2d.h>
#include "core/animation.h"

int twilight::AnimationManager::load(S2D_Sprite* sprite, const uint32_t width, const uint32_t height, const bool loop) {
    this->sprite = sprite;
    this->width = width;
    this->height = height;
    this->should_loop = loop;
    if(this->sprite) {
        return twilight::TWILIGHT_OK;
    }
    return twilight::TWILIGHT_ERROR;
}

int twilight::AnimationManager::start() {
    state = TWILIGHT_ANIM_RUNNING;
    return twilight::TWILIGHT_OK;
}

int twilight::AnimationManager::stop() {
    state = TWILIGHT_ANIM_STOP;
    return twilight::TWILIGHT_OK;
}

int twilight::AnimationManager::reset() {
    elapsed = 0.0;
    return twilight::TWILIGHT_OK;
}

void twilight::AnimationManager::update(double dt) {
    elapsed += (state == TWILIGHT_ANIM_RUNNING) ? dt : 0.0;
}

S2D_Sprite *twilight::AnimationManager::display() {
    double sec_per_frame = 1.0 / double(fps);
    auto frame = uint32_t(floor(elapsed / sec_per_frame));
    if(frame * width >= sprite->img->width) {
        if(should_loop) {
            reset();
        }
        else {
            state = TWILIGHT_ANIM_STOP;
        }
        return sprite;
    }
    S2D_ClipSprite(sprite, frame * width, 0, width, height);
    return sprite;
}
