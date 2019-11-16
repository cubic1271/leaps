#ifndef TWILIGHT_ANIMATION_H
#define TWILIGHT_ANIMATION_H

#define TWILIGHT_ANIM_DEFAULT_FPS  (12)

#include <cstdint>
#include <string>
#include <vector>

#include <simple2d/simple2d.h>
#include <Box2D/Box2D.h>

#include "core/util.h"

#define TWILIGHT_FONT_DEFAULT         "robotomono.ttf"
#define TWILIGHT_FONTSIZE_DEFAULT     (12)
#define TWILIGHT_DRIFTSPEED_DEFAULT   b2Vec2(0.0, -0.8)
#define TWILIGHT_FADELEN_DEFAULT      (3.0)
#define TWILIGHT_TEXTCOLOR_DEFAULT    ColorVec4(0.8, 0.8, 0.8, 1.0)

namespace twilight {
    constexpr int TWILIGHT_ANIM_RUNNING = 1;
    constexpr int TWILIGHT_ANIM_STOP    = 2;
    class AnimatedSprite {
    public:
        /**
         * Default constructor - initializes things to sane values
         */
        AnimatedSprite()
                : state(TWILIGHT_ANIM_STOP),
                  location(b2Vec2(0.0, 0.0)),
                  sprite(nullptr),
                  fps(TWILIGHT_ANIM_DEFAULT_FPS),
                  frames(0),
                  width(0),
                  height(0),
                  path(std::string("")),
                  elapsed(0.0),
                  should_loop(false) { }

        /**
         * Loads a sprite from a file as an animation that can run
         *
         * @param sprite the sprite object containing the animation (as a series of images)
         * @param width width of each individual frame
         * @param height height of each individual frame
         * @return twilight::OK on success, and twilight::ERROR on failure
         */
        int         load(S2D_Sprite* sprite, uint32_t width, uint32_t height, bool loop);

        /**
         * Starts an animation
         *
         * @return twilight::OK on success, and twilight::ERROR on failure
         */
        int         start();
        /**
         * Stops an animation
         *
         * @return twilight::OK on success, and twilight::ERROR on failure
         */
        int         stop();
        /**
         * Resets an animation
         *
         * @return twilight::OK on success, and twilight::ERROR on failure
         */
        int         reset();
        /**
         * Executed as part of the game update loop - used to update the current frame
         * @param dt elapsed time in seconds since the last time update was called
         */
        void        update(double dt);
        /**
         * Retrieves the current sprite as it should be displayed on the screen
         * @return a pointer to a sprite which can be drawn on the screen
         */
        S2D_Sprite* display();

        /**
         * Updates the world location of the sprite ...
         */
        void setLocation(b2Vec2 nLoc) {
            location = nLoc;
        }

        b2Vec2 getLocation() {
            return location;
        }

    protected:
        S2D_Sprite*   sprite;
        double        elapsed;
        b2Vec2        location;

        uint32_t      state;
        uint32_t      fps;
        uint32_t      frames;
        uint32_t      width;
        uint32_t      height;
        std::string   path;
        bool          should_loop;
    };

    class AnimatedText {
    public:
        static AnimatedText FadeTextDrift(std::string str, ColorVec4 color, b2Vec2 location, 
                                            double duration = TWILIGHT_FADELEN_DEFAULT, 
                                            b2Vec2 speed = TWILIGHT_DRIFTSPEED_DEFAULT, 
                                            std::string font = TWILIGHT_FONT_DEFAULT, 
                                            int size = TWILIGHT_FONTSIZE_DEFAULT);

        void start();
        void update(double dt);
        void render();
        int  construct(AnimatedText* config);
        void destroy();
        
        bool expired() {
            return timeElapsed > timeDuration;
        }

        void color(ColorVec4 from, ColorVec4 to) {
            colorStart = from;
            colorEnd = to;
        }

        void color(ColorVec4 target) {
            colorStart = target;
            colorEnd = target;
        }
        
        void location(b2Vec2 from, b2Vec2 to) {
            locationStart = from;
            locationEnd = to;
        }
        
        void location(b2Vec2 target) {
            locationStart = target;
            locationEnd = target;
        }
        
        void duration(double target) {
            timeDuration = target;
        }

    protected:
        S2D_Text*    text;
        std::string  str;
        std::string  font;
        int          size;
        double       timeElapsed;
        double       timeDuration;
        ColorVec4    colorStart;
        ColorVec4    colorEnd;
        b2Vec2       locationStart;
        b2Vec2       locationEnd;
    };

    class AnimationManager {
    public:
        typedef std::vector<AnimatedText*>    TextContainer;
        typedef std::vector<AnimatedSprite*>  SpriteContainer; 

        static AnimationManager* instance();

        void                     addText(AnimatedText* element);
        void                     addSprite(AnimatedSprite* sprite);
        void                     update(double dt);
        void                     render();

    protected:
        TextContainer    text;
        SpriteContainer  sprite;
    };
}

#endif //TWILIGHT_ANIMATION_H
