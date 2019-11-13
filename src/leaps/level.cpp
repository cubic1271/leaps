#include "leaps/level.h"
#include "core/resource.h"
#include "leaps/state.h"
#include <math.h>

static twilight::LevelManager* _manager = nullptr;

void twilight::Level::init(pugi::xml_document* level, b2World* world) {
    xml = level;
    this->world = world;

    ResourceManager* resource = ResourceManager::instance();
    pugi::xml_node base = xml->child("Level");
    title = base.child("Title").text().as_string();
    start.x = base.child("StartLocation").child("X").text().as_double();
    start.y = base.child("StartLocation").child("Y").text().as_double();
    for(auto label : base.children("WorldLabel")) {
        WorldLabel curr;
        curr.font = label.child("Font").text().as_string();
        curr.str = label.child("Text").text().as_string();
        curr.size = label.child("Size").text().as_int();
        curr.location.x = label.child("Location").child("X").text().as_double();
        curr.location.y = label.child("Location").child("Y").text().as_double();
        curr.text = resource->getText(curr.font, curr.size);
        curr.screenCoord = false;
        for(auto tv : label.child("Location").children("Absolute")) {
            printf("[level] loaded label with absolute coordinates ...\n");
            curr.screenCoord = true;
        }
        if(!curr.text) {
            printf("[level] failed to generate label: does the font exist?\n");
            continue;
        }
        S2D_SetText(curr.text, curr.str.c_str());
        worldLabel.push_back(curr);
    }

    for(auto image : base.children("WorldImage")) {
        WorldImage curr;
        curr.name = image.child("File").text().as_string();
        curr.location.x = image.child("Location").child("X").text().as_double();
        curr.location.y = image.child("Location").child("Y").text().as_double();
        curr.screenCoord = false;
        for(auto tv : image.child("Location").children("Absolute")) {
            printf("[level] loaded image with absolute coordinates ...\n");
            curr.screenCoord = true;
        }
        curr.image = resource->getImage(curr.name.c_str());
        if(!curr.image) {
            printf("[level] failed to load image `%s`: does the file exist?\n", curr.name.c_str());
            continue;
        }
        worldImage.push_back(curr);
    }

    for(auto wall : base.children("Wall")) {
        Wall* curr = new Wall;
        curr->type = curr->getTypeValue(wall.child("Type").text().as_string());
        curr->center.x = wall.child("Center").child("X").text().as_double();
        curr->center.y = wall.child("Center").child("Y").text().as_double();
        curr->dimension.x = wall.child("Width").text().as_double();
        curr->dimension.y = wall.child("Height").text().as_double();
        this->wall.push_back(curr);
        curr->constructWall(this->world);
    }

    printf("[level] loaded `%s`\n", title.c_str());
}

twilight::LevelManager* twilight::LevelManager::instance() {
    if(!_manager) {
        _manager = new LevelManager();
    }

    return _manager;
}

twilight::Level* twilight::LevelManager::load(std::string level, b2World* world) {
    ResourceManager* resource = ResourceManager::instance();
    pugi::xml_document* document = resource->getXml(std::string("level/" + level).c_str());
    if(!document) {
        printf("WARN: Failed to load level configuration: %s\n", std::string("level/" + level).c_str());
        return nullptr;
    }
    Level* target = new Level();
    target->init(document, world);
    return target;
}

void twilight::Level::update(double dt) {
    
}

void twilight::Level::render() {
    WorldProjection* proj = WorldProjection::instance();
    for(auto entry : worldLabel) {
        b2Vec2 target = proj->worldToScreen(entry.location);
        entry.text->x = target.x;
        entry.text->y = target.y;

        S2D_DrawText(entry.text);
    }

    for(auto entry : wall) {
        b2Vec2 topLeft = proj->worldToScreen(b2Vec2(entry->center.x - entry->dimension.x / 2.0, entry->center.y - entry->dimension.y / 2.0));
        b2Vec2 bottomRight = proj->worldToScreen(b2Vec2(entry->center.x + entry->dimension.x / 2.0, entry->center.y + entry->dimension.y / 2.0));
        S2D_DrawQuad(
            topLeft.x, topLeft.y, 0.8, 0.0, 0.0, 1.0,
            bottomRight.x, topLeft.y, 1.0, 0.0, 0.0, 1.0,
            bottomRight.x, bottomRight.y, 0.8, 0.0, 0.0, 1.0,
            topLeft.x, bottomRight.y, 1.0, 0.0, 0.0, 1.0);
    }
}

twilight::Wall::WallType twilight::Wall::getTypeValue(std::string type) {
    if(type == "outline") {
        return WALL_OUTLINE;
    }
    return WALL_UNKNOWN;
}

void twilight::Wall::constructWall(b2World* world) {
    b2BodyDef wall;
    wall.position = b2Vec2(center.x, center.y);
    wall.type = b2_staticBody;
    wall.userData = this;
    b2Body* tBody = world->CreateBody(&wall);
    b2FixtureDef tFix;
    b2PolygonShape* rect = new b2PolygonShape;
    rect->SetAsBox(dimension.x / 2.0, dimension.y / 2.0);
    tFix.shape = rect;
    tFix.friction = 0.6f;
    tFix.restitution = 0.2f;
    tBody->CreateFixture(&tFix);    
}
