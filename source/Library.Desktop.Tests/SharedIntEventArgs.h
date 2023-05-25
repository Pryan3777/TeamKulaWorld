#pragma once
#include "IEventArgs.h"

namespace LibraryDesktopTests {
    class SharedIntEventArgs final : public FieaGameEngine::IEventArgs {
        RTTI_DECLARATIONS(SharedIntEventArgs, FieaGameEngine::IEventArgs);

    public:
        inline explicit SharedIntEventArgs(int data = 0) : _data{std::make_shared<int>(data)} {};

        inline std::unique_ptr<IEventArgs> Clone() const override { return std::make_unique<SharedIntEventArgs>(*this); }

        std::shared_ptr<int> _data;

    };
}
