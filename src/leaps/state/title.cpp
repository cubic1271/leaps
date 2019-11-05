#include "core/twilight.h"
#include "core/resource.h"
#include "leaps/state.h"

namespace twilight {

void LeapsTitle::enter() {
    printf("Entering state: %s\n", name().c_str());
    S2D_PlayMusic(bgm, true);
}

void LeapsTitle::requestExit() {
    StateManager* manager = StateManager::instance();
    S2D_StopMusic();
    manager->completeTransition();
}

int LeapsTitle::init() {
    printf("Initializing state: %s\n", name().c_str());
    ResourceManager* resource = ResourceManager::instance();
    bgm = resource->getMusic(LEAPS_TITLE_BGM);
    if(nullptr == bgm) {
        printf("WARNING: unable to load bgm: %s\n", LEAPS_TITLE_BGM);
        return twilight::TWILIGHT_ERROR;
    }

    return twilight::TWILIGHT_OK;
}

void LeapsTitle::update(double dt) {
    
}

void LeapsTitle::render() {

}

void LeapsTitle::inputKey(S2D_Event* event) {

}

void LeapsTitle::inputMouse(S2D_Event* event) {

}

void LeapsTitle::inputController(S2D_Event* event) {

}

void LeapsTitle::BeginContact(b2Contact* contact) {

}

void LeapsTitle::EndContact(b2Contact* contact) {
    
}

}
