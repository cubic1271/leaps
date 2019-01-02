#include "core/state.h"
#include "core/twilight.h"

static twilight::StateManager* _manager = nullptr;
static twilight::StateEntry* _entry = nullptr;

twilight::StateManager *twilight::StateManager::instance() {
    if(nullptr == _manager) {
        _manager = new twilight::StateManager();
    }

    return _manager;
}

void twilight::StateManager::inputKey(S2D_Event *event) {

}

void twilight::StateManager::inputMouse(S2D_Event *event) {

}

int twilight::StateManager::addState(twilight::StateEntry *entry) {
    if(state.find(entry->name()) != state.end()) {
        return twilight::TWILIGHT_OK;
    }

    state[entry->name()] = entry;
    return twilight::TWILIGHT_OK;
}

twilight::StateEntry *twilight::StateManager::getCurrentState() {
    return current;
}

twilight::StateEntry *twilight::StateManager::getState(std::string name) {
    if(state.find(name) != state.end()) {
        return state[name];
    }

    return nullptr;
}

int twilight::StateManager::startTransition(twilight::StateEntry *next) {
    this->next = next;
    if(current == nullptr) {
        current = next;
    }
    else {
        current->requestExit();
    }
    return 0;
}

int twilight::StateManager::completeTransition() {
    current = next;
    current->enter();
    return 0;
}
