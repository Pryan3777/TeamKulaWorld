#pragma once
#include "IEventArgs.h"

namespace LibraryDesktopTests {
    class ScoreIncrementEventArgs final : public FieaGameEngine::IEventArgs
    {
        RTTI_DECLARATIONS(ScoreIncrementEventArgs, FieaGameEngine::IEventArgs);

    public:
        inline explicit ScoreIncrementEventArgs(int data = 0) : Increment(data) {};


        inline std::unique_ptr<IEventArgs> Clone() const override { return std::make_unique<ScoreIncrementEventArgs>(*this); }

        size_t Increment;
    };
}
