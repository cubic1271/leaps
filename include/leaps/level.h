#ifndef _LEAPS_LEVEL_H
#define _LEAPS_LEVEL_H

#include "simple2d/simple2d.h"
#include "Box2D/Box2D.h"
#include "pugixml/pugixml.hpp"
#include <vector>

namespace twilight {
    struct WorldLabel {
        std::string  str;
        S2D_Text*    text;
        std::string  font;
        int          size;
        b2Vec2       location;
        bool         screenCoord;
    };

    struct WorldImage {
        std::string  name;
        S2D_Image*   image;
        b2Vec2       location;
        bool         screenCoord;
    };

    struct Wall {
        enum WallType {
            WALL_OUTLINE = 0,
            WALL_UNKNOWN
        };

        WallType     type;
        b2Vec2       center;
        b2Vec2       dimension;

        WallType     getTypeValue(std::string type);
        void         constructWall(b2World* world);
    };

    class Level {
    public:
        typedef std::vector<WorldLabel>  LabelContainer;
        typedef std::vector<WorldImage>  ImageContainer;
        typedef std::vector<Wall>        WallContainer;
        void init(pugi::xml_document* level, b2World* world);
        void render();

    protected:
        pugi::xml_document*  xml;
        std::string          title;
        b2Vec2               start;
        LabelContainer       worldLabel;
        ImageContainer       worldImage;
        WallContainer        wall;
        b2World*             world;
    };

    class LevelManager {
    public:        
        static LevelManager* instance();

        Level* load(std::string level, b2World* world);

    };

}

#endif
