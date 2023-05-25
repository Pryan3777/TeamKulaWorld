#pragma once
#include "IEventArgs.h"

namespace FieaGameEngine
{
    class ClockEventArgs final : public FieaGameEngine::IEventArgs
    {
        RTTI_DECLARATIONS(ClockEventArgs, FieaGameEngine::IEventArgs);
    public:
        inline explicit ClockEventArgs(bool stopTime) : IsPaused(stopTime) {};

        inline std::unique_ptr<IEventArgs> Clone() const override { return std::make_unique<ClockEventArgs>(*this); }

        bool IsPaused = true;
    };
}
