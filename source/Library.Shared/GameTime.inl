#pragma once
#include "GameTime.h"

namespace FieaGameEngine {
    inline const std::chrono::high_resolution_clock::time_point& GameTime::CurrentTime() const { return _currentTime; }
    inline void GameTime::SetCurrentTime(const std::chrono::high_resolution_clock::time_point& currentTime) { _currentTime = currentTime; }
    inline const std::chrono::milliseconds& GameTime::TotalGameTime() const { return _totalGameTime; }
    inline void GameTime::SetTotalGameTime(const std::chrono::milliseconds& totalGameTime) { _totalGameTime = totalGameTime; }
    inline const std::chrono::milliseconds& GameTime::ElapsedGameTime() const { return _elapsedGameTime; }
    inline void GameTime::SetElapsedGameTime(const std::chrono::milliseconds& elapsedGameTime) { _elapsedGameTime = elapsedGameTime; }
    inline std::chrono::duration<float> GameTime::TotalGameTimeSeconds() const { return std::chrono::duration_cast<std::chrono::duration<float>>(_totalGameTime); }
    inline std::chrono::duration<float> GameTime::ElapsedGameTimeSeconds() const { return std::chrono::duration_cast<std::chrono::duration<float>>(_elapsedGameTime); }
}
