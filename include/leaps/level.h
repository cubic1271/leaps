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

    class Level {
    public:
        typedef std::vector<WorldLabel>  LabelContainer;
        typedef std::vector<WorldImage>  ImageContainer;
        void init(pugi::xml_document* level);
        void render();

    protected:
        pugi::xml_document*  xml;
        std::string          title;
        b2Vec2               start;
        LabelContainer       worldLabel;
        ImageContainer       worldImage;
    };

    class LevelManager {
    public:        
        static LevelManager* instance();

        Level* load(std::string level);

    };

    class WorldGrid {
    public:
        void    init(double w, double h, double res);
        int     world(double x, double y);
        int     at(int64_t x, int64_t y);

    protected:
        double   height;
        double   width;
        double   resolution;
        int64_t  cols;
        int64_t  rows;
        int*     grid;
    };
}

#endif
