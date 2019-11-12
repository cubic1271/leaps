#ifndef _LEAPS_ACTOR_H
#define _LEAPS_ACTOR_H

#include "core/state.h"
#include "Box2D/Box2D.h"

namespace twilight {
    class Actor {
    public:
        Actor();
        void          setWorld(b2World* target);
        b2Vec2        getWorldLocation();
        virtual void  setPhysics() = 0;
        virtual void  render() = 0;
        virtual void  inputKey(S2D_Event* event) = 0;
        virtual void  inputController(S2D_Event* event) = 0;

    protected:
        b2World*     world;
        b2Body*      body;
        S2D_Text*    debugText;
    };
}

#endif
