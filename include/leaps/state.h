#ifndef _LEAPS_STATE_H
#define _LEAPS_STATE_H

#include "core/state.h"

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
        S2D_Music* bgm;
};

}
#endif
