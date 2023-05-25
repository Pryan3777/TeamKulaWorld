#include "pch.h"
#include "GameClock.h"
#include "GameTime.h"

using namespace std::chrono;

namespace FieaGameEngine {
    void GameClock::Reset() {
        _startTime = high_resolution_clock::now();
        _currentTime = _startTime;
        _lastTime = _currentTime;
    }

    void GameClock::UpdateGameTime(GameTime& gameTime) {
        _currentTime = high_resolution_clock::now();

        gameTime.SetCurrentTime(_currentTime);
        gameTime.SetTotalGameTime(duration_cast<milliseconds>(_currentTime - _startTime));
        gameTime.SetElapsedGameTime(duration_cast<milliseconds>(_currentTime - _lastTime));
        _lastTime = _currentTime;
    }
}
