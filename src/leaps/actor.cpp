#include "leaps/actor.h"
#include "core/resource.h"

twilight::Actor::Actor() {
    ResourceManager* resource = ResourceManager::instance();
    debugText = resource->getText("aclonica.ttf", 12);
    assert(debugText);
    debugText->color.r = 1.0;
    debugText->color.g = 1.0;
    debugText->color.b = 1.0;
}

void twilight::Actor::render() {
    if(body) {
        WorldProjection* proj = WorldProjection::instance();
        b2Vec2 screenPos = proj->worldToScreen(body->GetPosition());
        S2D_DrawCircle(screenPos.x, screenPos.y, 8.0, 32, 0.5, 0.5, 1.0, 1.0);
        S2D_SetText(debugText, "Actor: world = (%0.2f, %0.2f), screen = (%0.2f, %0.2f)", 
                                body->GetPosition().x, body->GetPosition().y, 
                                screenPos.x, screenPos.y);
        S2D_DrawText(debugText);
    }
}

void twilight::Actor::inputKey(S2D_Event* event) {
    std::string key = event->key;
    b2Vec2 velocity = body->GetLinearVelocity();
    if(key == "A") {
        if(velocity.x > -2.0) {
            body->ApplyLinearImpulse(b2Vec2(-1.0, 0.0), body->GetWorldCenter(), true);
        }
    }
    else if(key == "D") {
        if(velocity.x < 2.0) {
            body->ApplyLinearImpulse(b2Vec2(1.0, 0.0), body->GetWorldCenter(), true);
        }
    }
    else if(key == "Space") { 
        if(abs(velocity.y < 0.3)) {
            body->ApplyLinearImpulse(b2Vec2(0.0, -5.0), body->GetWorldCenter(), true);
        }
    }
    else {
        // printf("[input] unsupported: %s\r\n", key.c_str());
    }
}

void twilight::Actor::inputController(S2D_Event* event) {

}

void twilight::Actor::setWorld(b2World* target) {
    world = target;
}

void twilight::Actor::setPhysics() {
    b2BodyDef info;
    info.type = b2_dynamicBody;
    info.userData = this;
    info.position = b2Vec2(5.0, 5.0);
    body = world->CreateBody(&info);

    b2CircleShape* tshape = new b2CircleShape;
    tshape->m_radius = 1.0;

    b2FixtureDef fixture;
    fixture.shape = tshape;
    fixture.density = 1.0f;
    fixture.friction = 0.7f;
    fixture.restitution = 0.1f;
    body->CreateFixture(&fixture);
    body->SetBullet(true);
}
