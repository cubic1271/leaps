#include "core/state.h"
#include "core/twilight.h"
#include <cassert>

#define TWILIGHT_B2_VELOCITY_ITER  (8)
#define TWILIGHT_B2_POSITION_ITER  (1)

static twilight::StateManager* _manager = nullptr;
static twilight::WorldProjection* _projection = nullptr;

twilight::WorldProjection::WorldProjection() {
    resolution = b2Vec2(0.0, 0.0);
    phySize = b2Vec2(0.0, 0.0);
    scale = b2Vec2(0.0, 0.0);
    invScale = b2Vec2(0.0, 0.0);
}

twilight::WorldProjection* twilight::WorldProjection::instance() {
    if(!_projection) {
        _projection = new WorldProjection;
    }

    return _projection;
}

void twilight::WorldProjection::set(b2Vec2 res, b2Vec2 phys) {
    assert(res.x > 0.01 && res.y > 0.01);
    resolution = res;
    phySize = phys;
    scale = b2Vec2(res.x / phys.x, res.y / phys.y);
    invScale = b2Vec2(1/scale.x, 1/scale.y);
}

void twilight::WorldProjection::updateCamera(b2Vec2 center) {
    offset.x = center.x - phySize.x / 2.0;
    offset.y = center.y - phySize.y / 2.0;
}

twilight::StateEntry::StateEntry() 
    : dt_accum(0.0), phyWorld(nullptr), updatePhysics(true) { 

}

void twilight::StateEntry::initPhysics() {
    phyWorld = new b2World(b2Vec2(0.0, TWILIGHT_GRAVITY_BASE));
    phyWorld->SetAllowSleeping(true);
    phyWorld->SetContinuousPhysics(true);
    phyWorld->SetContactListener(this);
}

void twilight::StateEntry::updateDelta(const double dt, const double tick) {
    dt_accum += dt;
    while(dt_accum > tick) {
        dt_accum -= tick;
        this->update(tick);
        if(updatePhysics && phyWorld) {
            phyWorld->Step(tick, TWILIGHT_B2_VELOCITY_ITER, TWILIGHT_B2_POSITION_ITER);
        }
    }
}

twilight::StateManager::StateManager() {
    current = nullptr;
    next = nullptr;
    transition_timer = 0.0;
    tick = TWILIGHT_TICK_DEFAULT;
}

twilight::StateManager *twilight::StateManager::instance() {
    if(nullptr == _manager) {
        _manager = new twilight::StateManager();
    }

    return _manager;
}

void twilight::StateManager::inputKey(S2D_Event *event) {
    if(current) {
        current->inputKey(event);
    }
}

void twilight::StateManager::inputMouse(S2D_Event *event) {
    if(current) {
        current->inputMouse(event);
    }
}

void twilight::StateManager::inputController(S2D_Event *event) {
    if(current) {
        current->inputController(event);
    }
}

void twilight::StateManager::update(double dt) {
    if(current) {
        current->updateDelta(dt, getTick());
    }
}

int twilight::StateManager::addState(twilight::StateEntry *entry) {
    if(state.find(entry->name()) != state.end()) {
        return twilight::TWILIGHT_OK;
    }

    assert(entry->init() == twilight::TWILIGHT_OK);
    state[entry->name()] = entry;
    return twilight::TWILIGHT_OK;
}

twilight::StateEntry *twilight::StateManager::getCurrentState() {
    return current;
}

twilight::StateEntry *twilight::StateManager::getState(std::string name) {
    if(state.find(name) != state.end()) {
        return state[name];
    }

    return nullptr;
}

int twilight::StateManager::startTransition(twilight::StateEntry *next) {
    this->next = next;
    if(current == nullptr) {
        completeTransition();
    }
    else {
        current->requestExit();
    }
    return 0;
}

int twilight::StateManager::completeTransition() {
    current = next;
    current->enter();
    return 0;
}
