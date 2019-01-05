#ifndef TWILIGHT_CONFIG_H
#define TWILIGHT_CONFIG_H

#include <string>

#define TWILIGHT_CONFIG_DEFAULT_PATH  "config.xml"

namespace twilight {
    class ConfigManager {
    public:
        static ConfigManager* instance();

        ConfigManager() : path(TWILIGHT_CONFIG_DEFAULT_PATH) { }

        void setPath(std::string path) { this->path = path; }
        std::string getPath() { return path; }

        std::string getSetting(std::string name);
        int getSettingAsInteger(std::string name);
        double getSettingAsFloat(std::string name);
        bool getSettingAsBoolean(std::string name);
    private:
        std::string path;
    };
}

#endif //TWILIGHT_CONFIG_H
