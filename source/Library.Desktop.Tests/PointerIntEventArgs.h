#pragma once
#include "IEventArgs.h"

namespace LibraryDesktopTests {
    class PointerIntEventArgs final : public FieaGameEngine::IEventArgs {
        RTTI_DECLARATIONS(PointerIntEventArgs, FieaGameEngine::IEventArgs);

    public:
        inline explicit PointerIntEventArgs(int* pointer = nullptr) : _pointer{pointer} {};

        inline std::unique_ptr<IEventArgs> Clone() const override { return std::make_unique<PointerIntEventArgs>(*this); }

        int* _pointer;

    };
}
