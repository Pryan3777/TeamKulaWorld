#pragma once
#include "IEventArgs.h"
#include "Foo.h"

namespace LibraryDesktopTests {
    class FooEventArgs final : public FieaGameEngine::IEventArgs {
        RTTI_DECLARATIONS(FooEventArgs, FieaGameEngine::IEventArgs);

    public:
        inline explicit FooEventArgs(int data = 0) : _foo{data} {};

        inline std::unique_ptr<IEventArgs> Clone() const override { return std::make_unique<FooEventArgs>(*this); }

        Foo _foo;

    };
}
