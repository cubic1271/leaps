#ifndef TWILIGHTENGINE_STATE_H
#define TWILIGHTENGINE_STATE_H

#include "Box2D/Box2D.h"
#include "simple2d/simple2d.h"
#include "core/twilight.h"
#include <string>
#include <map>

#define TWILIGHT_TICK_DEFAULT  (TWILIGHT_UPDATE_DELTA)  // 100 Hz

namespace twilight {
    class StateEntry : public b2ContactListener {
    public:
        StateEntry();
        virtual std::string name() = 0;

        virtual void enter() = 0;
        virtual void requestExit() = 0;
        virtual int  init() = 0;

        virtual void update(double dt) = 0;
        virtual void render() = 0;

        virtual void inputKey(S2D_Event* event) = 0;
        virtual void inputMouse(S2D_Event* event) = 0;
        virtual void inputController(S2D_Event* event) = 0;

        void         updateDelta(double dt, double tick);
        void         setPhysics(bool status) {
            updatePhysics = status;
        }
        void         initPhysics();

    protected:

        double      dt_accum;
        b2World*    phyWorld;
        bool        updatePhysics;
    };

    class StateManager {
    public:
        typedef std::map<std::string, StateEntry*> StateContainer;

        StateManager();

        static StateManager* instance();

        void inputKey(S2D_Event* event);
        void inputMouse(S2D_Event* event);
        void inputController(S2D_Event* event);

        int startTransition(StateEntry* next);
        int completeTransition();
        int addState(StateEntry* entry);
        StateEntry* getState(std::string name);
        StateEntry* getCurrentState();

        void update(double dt);

        void setTick(double t) {
            this->tick = t;
        }

        double getTick() {
            return this->tick;
        }

    private:
        StateContainer  state;
        StateEntry*     current;
        StateEntry*     next;
        double          transition_timer;
        double          tick;
    };

    class WorldProjection {
    public:
        static WorldProjection* instance();

        WorldProjection();
        void set(b2Vec2 res, b2Vec2 phys);

        b2Vec2 screenToWorld(b2Vec2 screen) {
            return b2Vec2(screen.x * invScale.x + offset.x, screen.y * invScale.y + offset.y);
        }

        b2Vec2 worldToScreen(b2Vec2 world) {
            return b2Vec2((world.x - offset.x) * scale.x, (world.y - offset.y) * scale.y);
        }

        double toScreenWidth(double x) {
            return x * scale.x;
        }

        double toScreenHeight(double y) {
            return y * scale.y;
        }

        b2Vec2 getResolution() {
            return resolution;
        }

        void updateCamera(b2Vec2 center);

    private:
        b2Vec2 resolution;
        b2Vec2 phySize;
        b2Vec2 scale;
        b2Vec2 invScale;

        b2Vec2 offset;
    };
}

#endif //TWILIGHTENGINE_STATE_H
