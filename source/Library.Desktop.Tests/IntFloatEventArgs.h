#pragma once
#include "IntEventArgs.h"

namespace LibraryDesktopTests {
    class IntFloatEventArgs final : public IntEventArgs {
        RTTI_DECLARATIONS(IntFloatEventArgs, IntEventArgs);

    public:
        inline explicit IntFloatEventArgs(float number = 0.f, int data = 0) : IntEventArgs{data}, _number{number} {};

        inline std::unique_ptr<IEventArgs> Clone() const override { return std::make_unique<IntFloatEventArgs>(*this); }

        float _number;

    };
}
