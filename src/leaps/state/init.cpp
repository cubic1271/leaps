#include "leaps/state.h"

namespace twilight {
    void LeapsStateInit() {
        StateManager* manager = StateManager::instance();
        LeapsTitle* title = new LeapsTitle();
        manager->addState(title);
    }
}
