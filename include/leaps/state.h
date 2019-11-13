#ifndef _LEAPS_STATE_H
#define _LEAPS_STATE_H

#include "core/state.h"
#include "core/console.h"
#include "leaps/level.h"
#include "leaps/actor.h"

namespace twilight {

void LeapsStateInit();

#define LEAPS_TITLE_BGM  "leaps.wav"
#define LEAPS_TITLE_NAME "state::Title"

class LeapsTitle : public StateEntry {
    public:
        std::string name() {
            return std::string(LEAPS_TITLE_NAME);
        };

        void enter();
        void requestExit();
        int  init();

        void update(double dt);
        void render();

        void inputKey(S2D_Event* event);
        void inputMouse(S2D_Event* event);
        void inputController(S2D_Event* event);
    protected:
        // Box2D callbacks.
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
        void PreSolve(b2Contact* contact);
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

        Level*     level;
        S2D_Music* bgm;
        S2D_Text*  debugText;
        Actor*     player;
        Console*   console;
        b2Vec2     mouseScreen;
        b2Vec2     mouseWorld;
};

}
#endif
