#ifndef TWILIGHT_PARTICLE_H
#define TWILIGHT_PARTICLE_H

#include <cmath>
#include <vector>
#include <map>
#include <string>

#include <simple2d/simple2d.h>
#include "util.h"

namespace twilight {
    class vec2 {
    public:
        double x;
        double y;

        vec2() : x(0), y(0) { }
        vec2(double x, double y) : x(x), y(y) { }
        vec2(vec2& target) = default;
        vec2(const vec2& target) = default;

        vec2& operator= (vec2 target) {
            if(&target != this) {
                x = target.x;
                y = target.y;
            }

            return *this;
        }

        vec2 operator+ (double scalar) {return {x + scalar, y + scalar};}
        vec2 operator+ (vec2 target) {return {x + target.x, y + target.y};}

        vec2 operator- (double scalar) {return {x - scalar, y - scalar};}
        vec2 operator- (vec2 target) {return {x - target.x, y - target.y};}

        vec2 operator* (double scalar) {return {x * scalar, y * scalar};}
        vec2 operator/ (double scalar) {return {x / scalar, y / scalar};}

        double  dot(vec2 target) {return x * target.x + y * target.y;}
        vec2    ccw() {return {-y, x}; }
        vec2    cw() {return  {y, -x}; }

        double magnitude() {
            return sqrt(x * x + y * y);
        }

        vec2 rotate(double beta) {
            return {
                    cos(beta) * x - sin(beta) * y,
                    sin(beta) * x + cos(beta) * y
            };
        }

        vec2 normalize() {
            double val = sqrt((x * x) + (y * y));
            return {x / val, y / val};
        }
    };

    /**
     * A 2-D equivalent of a camera.
     *
     * Takes a series of x, y coordinates in the simulation and projects them into appropriate positions on the screen -
     * this keeps us from having to use pixel values to drive our physical simulation, which is usually suboptimal.  It
     * also allows us to, for example, handle zoom and scroll without too much additional difficulty.
     */
    class Projector {
    public:
        Projector()
                :screen_width(0), screen_height(0), screen_x_offset(0), screen_y_offset(0),
                 simulation_width(0), simulation_height(0), simulation_x_offset(0), simulation_y_offset(0) { }

        void simulationToScreen(const vec2& simulation, vec2& screen);
        void screenToSimulation(const vec2& screen, vec2& simulation);

        void setScreen(int width, int height) { screen_width = width; screen_height = height; }
        void setScreenOffset(int x, int y) { screen_x_offset = x; screen_y_offset = y; }
        void setSimulation(double width, double height) { simulation_width = int(width); simulation_height = int(height); }
        void setSimulationOffset(double x, double y) {simulation_x_offset = int(x); simulation_y_offset = int(y); }

    private:
        int screen_width;
        int screen_height;
        int screen_x_offset;
        int screen_y_offset;

        int simulation_width;
        int simulation_height;
        int simulation_x_offset;
        int simulation_y_offset;
    };

    class Particle {
    public:
        vec2           position;
        vec2           velocity;
        vec2           acceleration;
        S2D_Sprite*    graphic;
        double         elapsed;
        double         lifetime;

        explicit Particle()
                : position{0, 0}, velocity{0, 0}, acceleration{0, 0}, graphic(nullptr), elapsed(0),
                  lifetime(0) { }
        explicit Particle(S2D_Sprite* graphic)
                : position{0, 0}, velocity{0, 0}, acceleration{0, 0}, graphic(graphic), elapsed(0),
                  lifetime(0) { }

        void setPosition(vec2 location) {this->position = location; }
        void setVelocity(vec2 velocity) {this->velocity = velocity; }
        void setAcceleration(vec2 acceleration) {this->acceleration = acceleration; }
        void setLifetime(double life) { this->lifetime = life; }
    };

    class ParticleSystem {
    public:
        static constexpr int   PARTICLE_SYSTEM_RUNNING = 0;
        static constexpr int   PARTICLE_SYSTEM_STOPPED = 1;

        void          setEmission(double emission) { this->emission = emission; }
        void          setLocation(vec2 location) { this->location = location; }
        void          setLifetime(double lifetime) { this->lifetime = lifetime; }
        void          setPersistent(bool persistent) { this->persistent = persistent; }
        void          setDirection(vec2 direction) { this->direction = direction; }
        void          setSpeed(double speed) { this->speed = speed; }
        void          setRadius(double radius) { this->radius = radius; }
        void          setState(int state) { this->state = state; }
        void          setName(std::string name) { this->name = name; }
        void          setAcceleration(vec2 acceleration) { this->acceleration = acceleration; }

        void          update(double dt);
        void          render();

        ParticleSystem()
                : state(PARTICLE_SYSTEM_STOPPED), emission(0), clock{ }, location{0, 0}, lifetime(0), systemLifetime(0),
                  persistent(false), direction{0.0, -1.0}, angle(0), speed(0), radius(5), acceleration{ }, graphics{ },
                  particles{ } {  }

    private:
        // Properties of the particle system itself
        int                           state;
        double                     emission;
        twilight::Timer               clock{};
        vec2                       location;
        double                     lifetime;
        double               systemLifetime;
        bool                     persistent;
        // Properties of the particles being emitted
        vec2                      direction;
        double                        angle;
        double                        speed;
        vec2                   acceleration;
        double                       radius;
        std::string                    name;

        std::vector<S2D_Sprite*>   graphics{};
        std::vector<Particle>      particles{};

        friend class ParticleManager;
    };

    class ParticleManager {
    public:
        typedef std::vector<ParticleSystem*> ParticleSystemContainer;
        typedef std::map<std::string, ParticleSystem*> ParticleSystemCache;
        static ParticleManager*  instance();

        ParticleSystem*          loadParticleSystem(std::string name);
        void                     update(double dt);
        void                     render();
        void                     clear();

    private:
        ParticleSystemContainer system;
        ParticleSystemCache cache;
    };
}

#endif //TWILIGHT_PARTICLE_H
