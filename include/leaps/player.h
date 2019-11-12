#ifndef _LEAPS_PLAYER_H
#define _LEAPS_PLAYER_H

#include "leaps/actor.h"

namespace twilight {
    class Player : public Actor {
    public:
        Player();
        void          setWorld(b2World* target);
        virtual void  setPhysics();

        virtual void  update(double dt);
        virtual void  render();
        virtual void  inputKey(S2D_Event* event);
        virtual void  inputController(S2D_Event* event);

        bool          isPlayer() { return true; }
        void          applyImpact(double impact);

    protected:
        double energy;
        double avatarSize;
    };
}

#endif
