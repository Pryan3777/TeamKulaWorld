#pragma once
#include "GameClock.h"

namespace FieaGameEngine {
    inline GameClock::GameClock() { Reset(); }

    inline const std::chrono::high_resolution_clock::time_point& GameClock::StartTime() const { return _startTime; }
    inline const std::chrono::high_resolution_clock::time_point& GameClock::CurrentTime() const { return _currentTime; }
    inline const std::chrono::high_resolution_clock::time_point& GameClock::LastTime() const { return _lastTime; }
}
