#include "pch.h"
#include "IEventArgs.h"

namespace FieaGameEngine {
    RTTI_DEFINITIONS(IEventArgs);
    RTTI_DEFINITIONS(InvalidEventArgs);

    std::unique_ptr<IEventArgs> InvalidEventArgs::Clone() const { return std::make_unique<InvalidEventArgs>(*this); }
}
