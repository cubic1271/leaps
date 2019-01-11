#include "core/util.h"

#include <cstdarg>
#include <cstdio>
#include <climits>

double twilight::Timer::total() {
    return ((std::chrono::duration_cast<std::chrono::microseconds>(_clock.now().time_since_epoch()) - _start).count()) / 1000000.0;
}

double twilight::Timer::elapsed() {
    return ((std::chrono::duration_cast<std::chrono::microseconds>(_clock.now().time_since_epoch()) - _last).count()) / 1000000.0;
}

void twilight::Timer::update() {
    _last = std::chrono::duration_cast<std::chrono::microseconds>(_clock.now().time_since_epoch());
}

void twilight::Timer::start() {
    _clock = std::chrono::high_resolution_clock();
    _start = std::chrono::duration_cast<std::chrono::microseconds>(_clock.now().time_since_epoch());
    _last = _start;
}
