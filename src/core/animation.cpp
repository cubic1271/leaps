#include <core/twilight.h>
#include <cmath>
#include <simple2d/simple2d.h>
#include "core/animation.h"
#include "core/resource.h"
#include "core/state.h"

static twilight::AnimationManager* _animation = nullptr;

int twilight::AnimatedSprite::load(S2D_Sprite* sprite, const uint32_t width, const uint32_t height, const bool loop) {
    this->sprite = sprite;
    this->width = width;
    this->height = height;
    this->should_loop = loop;
    if(this->sprite) {
        return twilight::TWILIGHT_OK;
    }
    return twilight::TWILIGHT_ERROR;
}

int twilight::AnimatedSprite::start() {
    state = TWILIGHT_ANIM_RUNNING;
    return twilight::TWILIGHT_OK;
}

int twilight::AnimatedSprite::stop() {
    state = TWILIGHT_ANIM_STOP;
    return twilight::TWILIGHT_OK;
}

int twilight::AnimatedSprite::reset() {
    elapsed = 0.0;
    return twilight::TWILIGHT_OK;
}

void twilight::AnimatedSprite::update(double dt) {
    elapsed += (state == TWILIGHT_ANIM_RUNNING) ? dt : 0.0;
}

S2D_Sprite *twilight::AnimatedSprite::display() {
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

void twilight::AnimatedText::update(double dt) {
    timeElapsed += dt;
}

int twilight::AnimatedText::construct(AnimatedText* config) {
    str = config->str;
    font = config->font;
    size = config->size;
    locationStart = config->locationStart;
    locationEnd = config->locationEnd;
    colorStart = config->colorStart;
    colorEnd = config->colorEnd;
    timeElapsed = 0.0;
    timeDuration = config->timeDuration;

    ResourceManager* resource = ResourceManager::instance();
    text = resource->getText(font, size);
    if(nullptr == text) {
        return twilight::TWILIGHT_ERROR;
    }
    S2D_SetText(text, str.c_str());

    return twilight::TWILIGHT_OK;
}

void twilight::AnimatedText::render() {
    WorldProjection* proj = WorldProjection::instance();
    double frac = timeElapsed / timeDuration;
    frac = frac > 1.0 ? 1.0 : frac;
    ColorVec4 tColor = colorStart.mix(colorEnd, frac);
    cv4_assign(text->color, tColor);
    b2Vec2 tLocation = b2Vec2(locationStart.x * (1 - frac) + locationEnd.x * (frac),
                              locationStart.y * (1 - frac) + locationEnd.y * (frac));
    tLocation = proj->worldToScreen(tLocation);
    text->x = tLocation.x;
    text->y = tLocation.y;
    S2D_DrawText(text);
}

void twilight::AnimatedText::destroy() {
    if(text) {
        delete text;
    }
}

twilight::AnimatedText twilight::AnimatedText::FadeTextDrift(std::string str, ColorVec4 color, b2Vec2 location, double duration, b2Vec2 speed, std::string font, int size) {
    AnimatedText result;
    result.str = str;
    result.font = font;
    result.size = size;
    result.colorStart = color;
    result.colorEnd = color;
    result.colorEnd.a = 0.0;
    result.locationStart = location;
    result.locationEnd = b2Vec2(
        location.x + (speed.x * duration),
        location.y + (speed.y * duration)
    );
    result.timeDuration = duration;
    return result;
}

void twilight::AnimationManager::update(double dt) {
    for(auto entry : text) {
        entry->update(dt);
    }

    auto iter = text.begin();

    while(iter != text.end()) {
        if((*iter)->expired()) {
            text.erase(iter);
        }
        else {
            ++iter;
        }
    }

/*
        text.erase(std::remove_if(
            text.begin(),
            text.end(),
            [dt](AnimatedText* element) -> bool {
                return element->expired();
            }
        ));
*/

    for(auto entry : sprite) {
        entry->update(dt);
    }
}

void twilight::AnimationManager::render() {
    for(auto entry : text) {
        entry->render();
    }

    WorldProjection* proj = WorldProjection::instance();
    for(auto entry : sprite) {
        S2D_Sprite* tSprite = entry->display();
        b2Vec2 tLocation = entry->getLocation();
        proj->worldToScreen(tLocation);
        tSprite->x = tLocation.x;
        tSprite->y = tLocation.y;
        S2D_DrawSprite(tSprite);
    }
}

void twilight::AnimationManager::addText(AnimatedText* aText) {
    AnimatedText* nText = new AnimatedText;
    nText->construct(aText);
    text.push_back(nText);
}

void twilight::AnimationManager::addSprite(AnimatedSprite* aSprite) {
    sprite.push_back(aSprite);
}

twilight::AnimationManager* twilight::AnimationManager::instance() {
    if(!_animation) {
        _animation = new AnimationManager;
    }
    return _animation;
}
