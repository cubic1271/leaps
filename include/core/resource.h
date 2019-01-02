#ifndef TWILIGHT_RESOURCE_H
#define TWILIGHT_RESOURCE_H

#include <map>
#include <simple2d/simple2d.h>

namespace twilight {
    struct FontCacheEntry {
        std::string              name;
        std::map<int, S2D_Text*> text;

        FontCacheEntry()
                : name("") { text = std::map<int, S2D_Text*>(); }
    };

    class ResourceManager {
    public:
        typedef std::map<std::string, S2D_Sprite*>      SpriteContainer;
        typedef std::map<std::string, S2D_Music*>       MusicContainer;
        typedef std::map<std::string, S2D_Sound*>       SoundContainer;
        typedef std::map<std::string, S2D_Image*>       ImageContainer;
        typedef std::map<std::string, FontCacheEntry>   TextContainer;

        void           init(std::string base);

        S2D_Sprite*    getSprite(std::string sprite);
        S2D_Image*     getImage(std::string image);
        S2D_Music*     getMusic(std::string music);
        S2D_Sound*     getSound(std::string sound);
        S2D_Text*      getText(std::string font, int size);

        static ResourceManager* instance();

    private:
        std::string base;

        SpriteContainer  sprite;
        MusicContainer   music;
        SoundContainer   sound;
        ImageContainer   image;
        TextContainer    text;
    };
}

#endif //TWILIGHT_RESOURCE_H
