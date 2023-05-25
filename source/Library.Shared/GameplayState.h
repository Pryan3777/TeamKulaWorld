#pragma once
#include "EventQueue.h"
#include "GameClock.h"
#include "GameTime.h"
#include "ReversePolishEvaluator.h"
#include "Scope.h"
#include "Stack.h"

namespace FieaGameEngine {
    class GameplayState final {

        friend std::unique_ptr<GameplayState> std::make_unique();

    public:
        static bool CreateSingleton();
        static bool DestroySingleton();
        static GameplayState& Singleton();

        GameplayState(const GameplayState&) = delete;
        GameplayState(GameplayState&&) noexcept = delete;
        GameplayState& operator=(const GameplayState&) = delete;
        GameplayState& operator=(GameplayState&&) noexcept = delete;
        ~GameplayState() = default;

        Stack<Scope>& ActionArgumentStack();
        EventQueue& Events();

        const GameTime& ProgressTime();
        const GameTime& ResetTime();

        [[nodiscard]] const GameClock& GetClock() const;
        [[nodiscard]] const GameTime& GetTime() const;

        const ReversePolishEvaluator& RpnEvaluator() const;

    private:
        static std::unique_ptr<GameplayState> _singleton;

        GameplayState();

        Stack<Scope> _actionArgumentStack{};
        EventQueue _events{};
        GameClock _clock{};
        GameTime _time{};
        ReversePolishEvaluator _rpnEval{};

    };
}

#include "GameplayState.inl"
