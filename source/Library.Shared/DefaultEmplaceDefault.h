#pragma once

namespace FieaGameEngine {
    template <typename T>
    struct DefaultEmplaceDefault {
        T operator()() const;
    };
}

#include "DefaultEmplaceDefault.inl"
