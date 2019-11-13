#include "core/twilight.h"
#include "core/resource.h"
#include "leaps/state.h"
#include "leaps/player.h"

#define LEAPS_TITLE_DEBUG_FONT "robotomono.ttf"

namespace twilight {

void LeapsTitle::enter() {
    printf("[state] entering: %s\n", name().c_str());
    S2D_PlayMusic(bgm, true);
}

void LeapsTitle::requestExit() {
    StateManager* manager = StateManager::instance();
    S2D_StopMusic();
    manager->completeTransition();
}

int LeapsTitle::init() {
    printf("[state] initializing: %s\n", name().c_str());
    ResourceManager* resource = ResourceManager::instance();
    LevelManager* levels = LevelManager::instance();
    bgm = resource->getMusic(LEAPS_TITLE_BGM);
    if(nullptr == bgm) {
        printf("[state] unable to load bgm: %s\n", LEAPS_TITLE_BGM);
        return twilight::TWILIGHT_ERROR;
    }
    debugText = resource->getText(LEAPS_TITLE_DEBUG_FONT, 14);
    S2D_SetText(debugText, "(0, 0)");
    debugText->x = 0;
    debugText->y = 0;
    initPhysics();
    level = nullptr;
    level = levels->load("title.xml", phyWorld);
    if(nullptr == level) {
        printf("[state] failed to load level data: %s", "title.xml");
        return twilight::TWILIGHT_ERROR;
    }
    player = new Player();
    player->setWorld(phyWorld);
    player->setPhysics();
    console = Console::instance();
    
    return twilight::TWILIGHT_OK;
}

void LeapsTitle::update(double dt) { 
    WorldProjection* proj = WorldProjection::instance();
    proj->updateCamera(player->getWorldLocation());

    mouseWorld = proj->screenToWorld(mouseScreen);
    S2D_SetText(debugText, "(%0.2f, %0.2f)", mouseWorld.x, mouseWorld.y);

    player->update(dt);
    level->update(dt);
}

void LeapsTitle::render() {
    player->render();
    level->render();
    console->render();
    S2D_DrawText(debugText);
}

void LeapsTitle::inputKey(S2D_Event* event) {
    player->inputKey(event);
}

void LeapsTitle::inputMouse(S2D_Event* event) {
    if(event->type == S2D_MOUSE_MOVE) {
        mouseScreen.x = event->x;
        mouseScreen.y = event->y;

        WorldProjection* proj = WorldProjection::instance();
        mouseWorld = proj->screenToWorld(mouseScreen);
        S2D_SetText(debugText, "(%0.2f, %0.2f)", mouseWorld.x, mouseWorld.y);
    }
    console->inputMouse(event);
}

void LeapsTitle::inputController(S2D_Event* event) {
    player->inputController(event);
}

void LeapsTitle::PreSolve(b2Contact* contact) {

}

void LeapsTitle::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    double aggregate = 0.0;
    for(int i = 0; i < impulse->count; ++i) {
        aggregate += impulse->normalImpulses[i];
    }
    ContactObject* objA = (ContactObject*)contact->GetFixtureA()->GetBody()->GetUserData();
    ContactObject* objB = (ContactObject*)contact->GetFixtureB()->GetBody()->GetUserData();

    objA->applyImpact(aggregate);
    objB->applyImpact(aggregate);
    if(aggregate > 10) {
        Console* console = Console::instance();
        console->write("Impact detected.");
    }
}

void LeapsTitle::BeginContact(b2Contact* contact) {

}

void LeapsTitle::EndContact(b2Contact* contact) {
    
}

}
