#ifndef TWILIGHT_UTIL_H
#define TWILIGHT_UTIL_H

#include <cstdint>
#include <chrono>

namespace twilight {
    class Timer {
    public:
        void      start();
        void     update();
        double  elapsed();
        double    total();

    private:
        std::chrono::high_resolution_clock  _clock;
        std::chrono::microseconds           _start;
        std::chrono::microseconds           _last;
    };
}

#endif //TWILIGHT_UTIL_H
