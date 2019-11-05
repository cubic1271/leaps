#include "core/resource.h"
#include "core/tinydir.h"

#define TWILIGHT_SPRITE_DIR  "sprite"
#define TWILIGHT_IMAGE_DIR   "image"
#define TWILIGHT_MUSIC_DIR   "music"
#define TWILIGHT_SOUND_DIR   "sound"
#define TWILIGHT_FONT_DIR    "font"

static twilight::ResourceManager* _manager = nullptr;

twilight::ResourceManager* twilight::ResourceManager::instance() {
    if(nullptr == _manager) {
        _manager = new ResourceManager();
    }

    return _manager;
}

void twilight::ResourceManager::init(std::string base) {
    this->base = base;
}

S2D_Sprite *twilight::ResourceManager::getSprite(std::string sprite) {
    if(this->sprite.find(sprite) != this->sprite.end()) {
        return this->sprite[sprite];
    }
    else {
        std::string path = this->base + "/" + TWILIGHT_SPRITE_DIR + "/" + sprite;
        S2D_Sprite* rval = S2D_CreateSprite(path.c_str());
        if(rval != nullptr) {
            this->sprite[sprite] = rval;
            return rval;
        }
        else {
            return nullptr;
        }
    }
}

S2D_Image *twilight::ResourceManager::getImage(std::string image) {
    if(this->image.find(image) != this->image.end()) {
        return this->image[image];
    }
    else {
        std::string path = this->base + "/" + TWILIGHT_IMAGE_DIR + "/" + image;
        S2D_Image* rval = S2D_CreateImage(path.c_str());
        if(rval != nullptr) {
            this->image[image] = rval;
            return rval;
        }
        else {
            return nullptr;
        }
    }
}

S2D_Music *twilight::ResourceManager::getMusic(std::string music) {
    if(this->music.find(music) != this->music.end()) {
        return this->music[music];
    }
    else {
        std::string path = this->base + "/" + TWILIGHT_MUSIC_DIR + "/" + music;
        S2D_Music* rval = S2D_CreateMusic(path.c_str());
        if(rval != nullptr) {
            this->music[music] = rval;
            return rval;
        }
        else {
            return nullptr;
        }
    }
}

S2D_Sound *twilight::ResourceManager::getSound(std::string sound) {
    if(this->sound.find(sound) != this->sound.end()) {
        return this->sound[sound];
    }
    else {
        std::string path = this->base + "/" + TWILIGHT_SOUND_DIR + "/" + sound;
        S2D_Sound* rval = S2D_CreateSound(path.c_str());
        if(rval != nullptr) {
            this->sound[sound] = rval;
            return rval;
        }
        else {
            return nullptr;
        }
    }
}

S2D_Text *twilight::ResourceManager::getText(std::string font, int size) {
    FontCacheEntry entry = this->text[font];
    if(entry.text.find(size) != entry.text.end()) {
        return entry.text[size];
    }
    else {
        std::string path = this->base + "/" + TWILIGHT_FONT_DIR + "/" + font;
        S2D_Text* text = S2D_CreateText(path.c_str(), "", size);
        if(text != nullptr) {
            entry.text[size] = text;
            return text;
        }
        else {
            return nullptr;
        }
    }
}

pugi::xml_document*  getXml(std::string path) {
    pugi::xml_document* document = new pugi::xml_document;
    pugi::xml_parse_result result = document->load_file(path.c_str());
    if(result.status != pugi::status_ok) {
        printf("Warning: failed to load resource file: %s\n", result.description());
        delete document;
        return nullptr;
    }

    return document;
}
