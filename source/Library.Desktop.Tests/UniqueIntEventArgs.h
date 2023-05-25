#pragma once
#include "IEventArgs.h"

namespace LibraryDesktopTests {
    class UniqueIntEventArgs final : public FieaGameEngine::IEventArgs {
        RTTI_DECLARATIONS(UniqueIntEventArgs, FieaGameEngine::IEventArgs);

    public:
        inline explicit UniqueIntEventArgs(int data = 0) : _data{std::make_unique<int>(data)} {};
        inline UniqueIntEventArgs(const UniqueIntEventArgs&) = delete;
        inline UniqueIntEventArgs(UniqueIntEventArgs&&) noexcept = default;
        inline UniqueIntEventArgs& operator=(const UniqueIntEventArgs&) = delete;
        inline UniqueIntEventArgs& operator=(UniqueIntEventArgs&&) noexcept = default;
        inline ~UniqueIntEventArgs() = default;

        inline std::unique_ptr<IEventArgs> Clone() const override { return std::make_unique<UniqueIntEventArgs>(*_data); }

        std::unique_ptr<int> _data;

    };
}
