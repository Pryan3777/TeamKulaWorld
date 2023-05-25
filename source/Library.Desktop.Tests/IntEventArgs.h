#pragma once
#include "IEventArgs.h"

namespace LibraryDesktopTests {
    class IntEventArgs : public FieaGameEngine::IEventArgs {
        RTTI_DECLARATIONS(IntEventArgs, FieaGameEngine::IEventArgs);

    public:
        inline explicit IntEventArgs(int data = 0) : _data{data} {};
        virtual ~IntEventArgs() = default;

        inline virtual std::unique_ptr<IEventArgs> Clone() const override { return std::make_unique<IntEventArgs>(*this); }

        int _data;

    };
}
