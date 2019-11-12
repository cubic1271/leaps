#include "leaps/actor.h"
#include "core/resource.h"

twilight::Actor::Actor() {

}

b2Vec2 twilight::Actor::getWorldLocation() {
    return body->GetWorldCenter();
}

void twilight::Actor::setWorld(b2World* target) {
    world = target;
}
