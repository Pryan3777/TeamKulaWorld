#pragma once
#include <chrono>

namespace FieaGameEngine {
    class GameTime final {

    public:
        [[nodiscard]] const std::chrono::high_resolution_clock::time_point& CurrentTime() const;
        void SetCurrentTime(const std::chrono::high_resolution_clock::time_point& currentTime);
        
        [[nodiscard]] const std::chrono::milliseconds& TotalGameTime() const;
        void SetTotalGameTime(const std::chrono::milliseconds& totalGameTime);

        [[nodiscard]] const std::chrono::milliseconds& ElapsedGameTime() const;
        void SetElapsedGameTime(const std::chrono::milliseconds& elapsedGameTime);

        [[nodiscard]] std::chrono::duration<float> TotalGameTimeSeconds() const;
        [[nodiscard]] std::chrono::duration<float> ElapsedGameTimeSeconds() const;

    private:
        std::chrono::high_resolution_clock::time_point _currentTime{};
        std::chrono::milliseconds _totalGameTime{0};
        std::chrono::milliseconds _elapsedGameTime{0};

    };
}

#include "GameTime.inl"
