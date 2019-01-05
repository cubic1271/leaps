#include <core/resource.h>
#include "core/particle.h"
#include "pugixml/pugixml.hpp"

#define TWILIGHT_PARTICLE_DIR  "particle"

static twilight::ParticleManager* _manager = nullptr;

void twilight::ParticleSystem::render() {
    for(const auto& iter : particles) {
        iter.graphic->x = uint32_t(iter.position.x);
        iter.graphic->y = uint32_t(iter.position.y);

        S2D_DrawSprite(iter.graphic);
    }
}

void twilight::ParticleSystem::update(double dt) {
    for(auto iter = particles.begin(); iter != particles.end(); ++iter) {
        iter->elapsed = iter->elapsed + dt;
        if(iter->elapsed > iter->lifetime) {
            iter = particles.erase(iter);
        }

        iter->velocity = iter->velocity + (iter->acceleration * dt);
        iter->position = iter->position + (iter->velocity * dt);
    }
}

void twilight::ParticleManager::render() {

}

void twilight::ParticleManager::update(double dt) {
    for(auto iter : system) {
        iter->update(dt);
    }
}

void twilight::ParticleManager::clear() {

}

twilight::ParticleSystem* twilight::ParticleManager::createParticleSystem() {
    auto system = new ParticleSystem;
    this->system.push_back(system);
    return system;
}

twilight::ParticleSystem* twilight::ParticleManager::loadParticleSystem(std::string name) {
    pugi::xml_document document;
    ResourceManager* manager = ResourceManager::instance();
    std::string path = manager->getBase() + "/" + TWILIGHT_PARTICLE_DIR + "/" + name + ".xml";
    pugi::xml_parse_result result = document.load_file(path.c_str());
    if(result.status != pugi::status_ok) {
        return nullptr;
    }
    if(std::string(document.first_child().name()) != "ParticleSystem") {
        return nullptr;
    }
    pugi::xml_node systemNode = document.child("ParticleSystem");
    auto system = new ParticleSystem;
    system->persistent = systemNode.attribute("persistent").as_bool(false);
    double dir_x = atof(systemNode.child("Direction").child("X").child_value());
    double dir_y = atof(systemNode.child("Direction").child("Y").child_value());
    if(!system->persistent) {
        system->lifetime = atof(systemNode.child("Lifetime").child_value());
        if(system->lifetime < 0.001) {
            printf("Invalid particle system: not persistent and <= 0.001 second lifetime.\n");
            delete system;
            return nullptr;
        }
    }
    system->speed = atof(systemNode.child("Speed").child_value());
    system->radius = atof(systemNode.child("Radius").child_value());

    return system;
}

twilight::ParticleManager *twilight::ParticleManager::instance() {
    if(nullptr == _manager) {
        _manager = new twilight::ParticleManager();
    }
    return _manager;
}

void twilight::Projector::screenToSimulation(const twilight::vec2 &screen, twilight::vec2 &simulation) {
    vec2 adjusted = screen;

    adjusted.x -= screen_x_offset;
    adjusted.x /= screen_width;
    adjusted.x *= simulation_width;
    adjusted.x += simulation_x_offset;

    adjusted.y -= screen_y_offset;
    adjusted.y /= screen_height;
    adjusted.y *= simulation_height;
    adjusted.y += simulation_y_offset;

    simulation.x = adjusted.x;
    simulation.y = adjusted.y;
}

void twilight::Projector::simulationToScreen(const twilight::vec2 &simulation, twilight::vec2 &screen) {
    vec2 adjusted = simulation;

    adjusted.x -= simulation_x_offset;
    adjusted.x /= simulation_width;
    adjusted.x *= screen_width;
    adjusted.x += screen_x_offset;

    adjusted.y -= simulation_y_offset;
    adjusted.y /= simulation_height;
    adjusted.y *= screen_height;
    adjusted.y += screen_y_offset;

    screen.x = adjusted.x;
    screen.y = adjusted.y;
}
