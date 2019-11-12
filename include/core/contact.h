#ifndef _TWILIGHT_CONTACT_H
#define _TWILIGHT_CONTACT_H

#include "core/util.h"
#include "Box2D/Box2D.h"

namespace twilight {
    class ContactObject {
    public:
        virtual bool    isWall() = 0;
        virtual bool    isPlayer() = 0;
        virtual bool    isActor() = 0;
        virtual bool    isProjectile() = 0;

        virtual void    applyImpact(double force) { }
    };
}

#endif
