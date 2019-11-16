#ifndef TWILIGHT_UTIL_H
#define TWILIGHT_UTIL_H

#include <cstdint>
#include <chrono>

#define cv4_assign(target, cv4)  target.r = cv4.r; target.g = cv4.g; target.b = cv4.b; target.a = cv4.a
#define cv4_expand(cv4)  cv4.r, cv4.g, cv4.b, cv4.a

namespace twilight {
    struct ColorVec4 {
        double r;
        double g;
        double b;
        double a;

        ColorVec4()
        : r(0.0), g(0.0), b(0.0), a(0.0) { }

        ColorVec4(double tr, double tg, double tb, double ta) 
        : r(tr), g(tg), b(tb), a(ta) { }

        ColorVec4(ColorVec4 const& v2)
        : r(v2.r), g(v2.g), b(v2.b), a(v2.a) { }

        ColorVec4 mix(ColorVec4 target, double frac) {
            return ColorVec4(
                r * (1 - frac) + target.r * (frac),
                g * (1 - frac) + target.g * (frac),
                b * (1 - frac) + target.b * (frac),
                a * (1 - frac) + target.a * (frac)
            );
        }

        ColorVec4 operator* (double scalar) const {
            return ColorVec4(scalar * r, scalar * g, scalar * b, scalar * a);
        }

        ColorVec4 operator+ (ColorVec4 v2) const {
            return ColorVec4(r + v2.r, g + v2.g, b + v2.b, a + v2.a);
        }
    };

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
