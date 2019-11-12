#include "core/twilight.h"
#include "core/resource.h"
#include "leaps/state.h"
#include "leaps/player.h"

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

/*
    b2BodyDef bottomWall;
    bottomWall.position = b2Vec2(0, 25);
    bottomWall.type = b2_staticBody;
    bottomWall.userData = this;
    b2Body* tBody = phyWorld->CreateBody(&bottomWall);
    b2FixtureDef tFix;
    b2PolygonShape* rect = new b2PolygonShape;
    rect->SetAsBox(50.0, 1.0);
    tFix.shape = rect;
    tFix.friction = 0.6f;
    tFix.restitution = 0.2f;
    tBody->CreateFixture(&tFix);
*/

    return twilight::TWILIGHT_OK;
}

void LeapsTitle::update(double dt) { 
    WorldProjection* proj = WorldProjection::instance();
    proj->updateCamera(player->getWorldLocation());
}

void LeapsTitle::render() {
    player->render();
    level->render();
}

void LeapsTitle::inputKey(S2D_Event* event) {
    player->inputKey(event);
}

void LeapsTitle::inputMouse(S2D_Event* event) {

}

void LeapsTitle::inputController(S2D_Event* event) {
    player->inputController(event);
}

void LeapsTitle::BeginContact(b2Contact* contact) {
}

void LeapsTitle::EndContact(b2Contact* contact) {
    
}

}
