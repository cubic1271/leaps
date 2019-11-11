#include "leaps/level.h"
#include "core/resource.h"
#include "leaps/state.h"
#include <math.h>

static twilight::LevelManager* _manager = nullptr;

void twilight::WorldGrid::init(double w, double h, double res) {
    printf("Generating world: %f x %f, steps of %f\n");
    width = w;
    height = h;
    resolution = res;

    cols = w / res;
    rows = h / res;
    grid = new int[cols * rows];
}

int  twilight::WorldGrid::at(int64_t x, int64_t y) {
    return grid[y * cols + x];
}

int  twilight::WorldGrid::world(double x, double y) {
    int64_t col = x / resolution;
    int64_t row = y / resolution;
    return grid[row * cols + col];
}

void twilight::Level::init(pugi::xml_document* level) {
    xml = level;

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

    printf("[level] loaded `%s`\n", title.c_str());
}

twilight::LevelManager* twilight::LevelManager::instance() {
    if(!_manager) {
        _manager = new LevelManager();
    }

    return _manager;
}

twilight::Level* twilight::LevelManager::load(std::string level) {
    ResourceManager* resource = ResourceManager::instance();
    pugi::xml_document* document = resource->getXml(std::string("level/" + level).c_str());
    if(!document) {
        printf("WARN: Failed to load level configuration: %s\n", std::string("level/" + level).c_str());
        return nullptr;
    }
    Level* target = new Level();
    target->init(document);
    return target;
}

void twilight::Level::render() {
    WorldProjection* proj = WorldProjection::instance();
    for(auto entry : worldLabel) {
        b2Vec2 target = proj->worldToScreen(entry.location);
        entry.text->x = target.x;
        entry.text->y = target.y;

        S2D_DrawText(entry.text);
    }
}
