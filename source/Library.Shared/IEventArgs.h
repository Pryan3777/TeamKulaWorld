#pragma once
#include "RTTI.h"
#include <memory>

namespace FieaGameEngine {
    class IEventArgs : public RTTI {
        RTTI_DECLARATIONS(IEventArgs, RTTI);

    public:
        virtual ~IEventArgs() = default;

        [[nodiscard]] virtual std::unique_ptr<IEventArgs> Clone() const = 0;

    };

    class InvalidEventArgs final : public IEventArgs {
        RTTI_DECLARATIONS(InvalidEventArgs , IEventArgs);

    public:
        [[nodiscard]] virtual std::unique_ptr<IEventArgs> Clone() const override;

    };
}
