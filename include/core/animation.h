#ifndef TWILIGHTENGINE_ANIMATION_H
#define TWILIGHTENGINE_ANIMATION_H

#define TWILIGHT_ANIM_DEFAULT_FPS  (12)

#include <cstdint>
#include <string>
#include <simple2d/simple2d.h>

namespace twilight {
    constexpr int TWILIGHT_ANIM_RUNNING = 1;
    constexpr int TWILIGHT_ANIM_STOP    = 2;
    class AnimationManager {
    public:
        uint32_t      state;
        uint32_t      fps;
        uint32_t      frames;
        uint32_t      width;
        uint32_t      height;
        std::string   path;
        bool          should_loop;

        /**
         * Default constructor - initializes things to sane values
         */
        AnimationManager()
                : state(TWILIGHT_ANIM_STOP),
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

    private:
        S2D_Sprite*   sprite;
        double        elapsed;
    };
}

#endif //TWILIGHTENGINE_ANIMATION_H
