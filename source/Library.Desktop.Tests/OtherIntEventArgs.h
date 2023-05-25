#pragma once
#include "IEventArgs.h"

namespace LibraryDesktopTests {
    class OtherIntEventArgs final : public FieaGameEngine::IEventArgs {
        RTTI_DECLARATIONS(OtherIntEventArgs, FieaGameEngine::IEventArgs);

    public:
        inline explicit OtherIntEventArgs(int data = 0) : _data{data} {};

        inline std::unique_ptr<IEventArgs> Clone() const override { return std::make_unique<OtherIntEventArgs>(*this); }

        int _data;

    };
}
