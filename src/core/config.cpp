#include "core/config.h"
#include "core/resource.h"
#include "pugixml/pugixml.hpp"

static twilight::ConfigManager* _manager = nullptr;

using twilight::ResourceManager;

twilight::ConfigManager* twilight::ConfigManager::instance() {
    if(_manager == nullptr) {
        _manager = new ConfigManager;
    }

    return _manager;
}

std::string twilight::ConfigManager::getSetting(std::string name) {
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    if(result.status != pugi::status_ok) {
        printf("Warning: failed to load configuration file: %s\n", result.description());
        return std::string("");
    }

    pugi::xml_node config = document.child("SystemConfig");
    return std::string(config.child(name.c_str()).child_value());
}

int twilight::ConfigManager::getSettingAsInteger(std::string name) {
    return atoi(getSetting(name).c_str());
}

double twilight::ConfigManager::getSettingAsFloat(std::string name) {
    return atof(getSetting(name).c_str());
}

bool twilight::ConfigManager::getSettingAsBoolean(std::string name) {
    std::string raw = getSetting(name);
    return (raw == "true") || (getSettingAsInteger(name) > 0);
}
