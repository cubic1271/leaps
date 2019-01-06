#include <core/resource.h>
#include "core/particle.h"
#include "pugixml/pugixml.hpp"

#define TWILIGHT_PARTICLE_DIR  "particle"
#define TWILIGHT_PARTICLE_GRAPHIC_DIR  "particle/graphic"

static twilight::ParticleManager* _manager = nullptr;

static double _random_shift() {
    return (rand() / double(RAND_MAX)) - 0.5;
}

static double _d2r(double d) {
    return (d * M_PI / 180.0);
}

static double _r2d(double d) {
    return (d * 180.0 / M_PI);
}

void twilight::ParticleSystem::render() {
    for(const auto& iter : particles) {
        iter.graphic->x = uint32_t(iter.position.x);
        iter.graphic->y = uint32_t(iter.position.y);

        iter.graphic->color.a = 1.0f - GLfloat(iter.elapsed / iter.lifetime);
        S2D_DrawSprite(iter.graphic);
    }
}

void twilight::ParticleSystem::update(double dt) {
    if(clock.elapsed() > emission) {
        vec2 target = direction.normalize().rotate(_random_shift() * _d2r(angle));
        Particle tmp;
        tmp.lifetime = lifetime;
        tmp.position.x = location.x;
        tmp.position.y = location.y;
        tmp.velocity.x = speed * target.x;
        tmp.velocity.y = speed * target.y;
        tmp.acceleration.x = acceleration.x;
        tmp.acceleration.y = acceleration.y;
        tmp.graphic = graphics[rand() % graphics.size()];
        particles.push_back(tmp);
        clock.update();
    }

    for(auto iter = particles.begin(); iter != particles.end(); ) {
        iter->elapsed = iter->elapsed + dt;
        if(iter->elapsed > iter->lifetime) {
            iter = particles.erase(iter);
            continue;
        }
        iter->velocity = iter->velocity + (iter->acceleration * dt);
        iter->position = iter->position + (iter->velocity * dt);
        iter++;
    }
}

void twilight::ParticleManager::render() {
    for(auto iter : system) {
        iter->render();
    }
}

void twilight::ParticleManager::update(double dt) {
    for(auto iter : system) {
        iter->update(dt);
    }
}

void twilight::ParticleManager::clear() {

}

twilight::ParticleSystem* twilight::ParticleManager::loadParticleSystem(std::string name) {
    // if we've loaded this before, use what we have in our cache
    if(cache.find(name) != cache.end()) {
        auto system = new ParticleSystem;
        ParticleSystem* basis = cache[name];

        system->angle = basis->angle;
        system->emission = basis->emission;
        system->acceleration = basis->acceleration;
        system->direction = basis->direction;
        system->speed = basis->speed;
        system->lifetime = basis->lifetime;
        system->systemLifetime = basis->systemLifetime;
        system->persistent = basis->persistent;
        system->name = basis->name;
        for(auto graphic : basis->graphics) {
            system->graphics.push_back(graphic);
        }
        system->location = basis->location;
        system->clock.start();

        this->system.push_back(system);
        return system;
    }

    printf("Loading new particle system: %s (%d)\n", name.c_str(), (int)cache.size());
    // if we haven't loaded this before, try to do so and then call this method again.
    pugi::xml_document document;
    ResourceManager* manager = ResourceManager::instance();
    std::string path = manager->getBase() + "/" + TWILIGHT_PARTICLE_DIR + "/" + name + ".xml";
    std::string graphic_path = manager->getBase() + "/" + TWILIGHT_PARTICLE_GRAPHIC_DIR;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    if(result.status != pugi::status_ok) {
        return nullptr;
    }
    if(std::string(document.first_child().name()) != "ParticleSystem") {
        return nullptr;
    }
    pugi::xml_node systemNode = document.child("ParticleSystem");
    auto system = new ParticleSystem;
    system->name = name;
    system->persistent = systemNode.attribute("persistent").as_bool(false);
    system->emission = atof(systemNode.child("Emission").child_value());
    system->angle = atof(systemNode.child("Angle").child_value());
    double dir_x = atof(systemNode.child("Direction").child("X").child_value());
    double dir_y = atof(systemNode.child("Direction").child("Y").child_value());
    system->direction = vec2(dir_x, dir_y);
    double accel_x = atof(systemNode.child("Acceleration").child("X").child_value());
    double accel_y = atof(systemNode.child("Acceleration").child("Y").child_value());
    system->acceleration = vec2(accel_x, accel_y);
    system->lifetime = atof(systemNode.child("Lifetime").child_value());
    if(!system->persistent) {
        system->systemLifetime = atof(systemNode.child("SystemLifetime").child_value());
        if(system->systemLifetime < 0.001) {
            printf("Invalid particle system: not persistent and <= 0.001 second SystemLifetime.\n");
            delete system;
            return nullptr;
        }
    }
    system->speed = atof(systemNode.child("Speed").child_value());
    system->radius = atof(systemNode.child("Radius").child_value());
    pugi::xml_node sprites = systemNode.child("Sprites");

    for (pugi::xml_node sprite = sprites.child("Sprite"); sprite; sprite = sprite.next_sibling("Sprite"))
    {
        printf("Loading sprite: %s\n", std::string(graphic_path + "/" + sprite.child_value()).c_str());
        S2D_Sprite* current = S2D_CreateSprite(std::string(graphic_path + "/" + sprite.child_value()).c_str());
        if(nullptr != current) {
            system->graphics.push_back(current);
        }
        else {
            printf("Failed to load sprite: %s\n", std::string(graphic_path + "/" + sprite.child_value()).c_str());
        }
    }
    if(system->graphics.size() == 0) {
        printf("Unable to load any particle graphics - load failed ...");
        delete system;
        return nullptr;
    }

    cache[name] = system;
    return loadParticleSystem(name);  // now that we've cached the system, go ahead and actually instantiate it.
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
