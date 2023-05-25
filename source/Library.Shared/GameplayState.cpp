#include "pch.h"
#include "GameplayState.h"

namespace FieaGameEngine {
    std::unique_ptr<GameplayState> GameplayState::_singleton{};

    bool GameplayState::CreateSingleton() {
        bool isCreating = !_singleton;

        if (isCreating) {
            _singleton = std::make_unique<GameplayState>();
        }

        return isCreating;
    }

    GameplayState& GameplayState::Singleton() {
        if (!_singleton) {
            using namespace std::literals::string_literals;

            throw std::logic_error("GameplayState singleton has not been created, cannot retrieve the reference!"s);
        }

        return *_singleton;
    }
}
