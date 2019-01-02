#ifndef TWILIGHTENGINE_STATE_H
#define TWILIGHTENGINE_STATE_H

#include "simple2d/simple2d.h"
#include <string>
#include <map>

namespace twilight {
    class StateEntry {
    public:

        virtual std::string name() = 0;

        virtual void enter() = 0;
        virtual void requestExit() = 0;
        virtual void init() = 0;

        virtual void update(double dt) = 0;
        virtual void render() = 0;

        virtual void inputKey(S2D_Event* event) = 0;
        virtual void inputMouse(S2D_Event* event) = 0;
    };

    class StateManager {
    public:
        typedef std::map<std::string, StateEntry*> StateContainer;

        static StateManager* instance();

        void inputKey(S2D_Event* event);
        void inputMouse(S2D_Event* event);

        int startTransition(StateEntry* next);
        int completeTransition();
        int addState(StateEntry* entry);
        StateEntry* getState(std::string name);
        StateEntry* getCurrentState();

    private:
        StateContainer  state;
        StateEntry*     current;
        StateEntry*     next;
        double          transition_timer;
    };
}

#endif //TWILIGHTENGINE_STATE_H
