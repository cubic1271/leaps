#include "leaps/player.h"
#include "core/resource.h"
#include "core/animation.h"

twilight::Player::Player() {
    ResourceManager* resource = ResourceManager::instance();
    avatarSize = 0.4;
}

void twilight::Player::render() {
    if(body) {
        WorldProjection* proj = WorldProjection::instance();
        b2Vec2 screenPos = proj->worldToScreen(body->GetPosition());
        double absRadius = proj->toScreenHeight(body->GetPosition().y + avatarSize) - proj->toScreenHeight(body->GetPosition().y); 
        S2D_DrawCircle(screenPos.x, screenPos.y, absRadius, 32, 1.0, 1.0, 1.0, 1.0);
    }
}

void twilight::Player::update(double dt) {

}

void twilight::Player::inputKey(S2D_Event* event) {
    std::string key = event->key;
    b2Vec2 velocity = body->GetLinearVelocity();
    if(key == "A") {
        if(velocity.x > -4.0) {
            body->ApplyLinearImpulse(b2Vec2(-1.0, 0.0), body->GetWorldCenter(), true);
        }
    }
    else if(key == "D") {
        if(velocity.x < 4.0) {
            body->ApplyLinearImpulse(b2Vec2(1.0, 0.0), body->GetWorldCenter(), true);
        }
    }
    else if(key == "Space") { 
        b2ContactEdge* contacts = body->GetContactList();
        if(contacts && abs(velocity.y) < 5) {
            body->ApplyLinearImpulse(b2Vec2(0.0, -4.0), body->GetWorldCenter(), true);
        }
    }
    else {
        // printf("[input] unsupported: %s\r\n", key.c_str());
    }
}

void twilight::Player::inputController(S2D_Event* event) {

}

void twilight::Player::setPhysics() {
    b2BodyDef info;
    info.type = b2_dynamicBody;
    info.userData = this;
    info.position = b2Vec2(5.0, 5.0);
    body = world->CreateBody(&info);

    b2CircleShape* tshape = new b2CircleShape;
    tshape->m_radius = avatarSize;

    b2FixtureDef fixture;
    fixture.shape = tshape;
    fixture.density = 1.0f;
    fixture.friction = 0.7f;
    fixture.restitution = 0.1f;
    body->CreateFixture(&fixture);
    body->SetBullet(true);
}

void twilight::Player::applyImpact(double impact) {
    if(impact > 6.0) {
        b2Vec2 playLocation = getWorldLocation();
        playLocation.y -= 3 * avatarSize;
        AnimatedText impactText = AnimatedText::FadeTextDrift("OUCH!", ColorVec4(1.0, 0.8, 0.8, 1.0), playLocation);
        AnimationManager* animation = AnimationManager::instance();
        animation->addText(&impactText);
    }
}
