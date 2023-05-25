#pragma once
#include "DefaultEmplaceDefault.h"

namespace FieaGameEngine {
    template <typename T> T DefaultEmplaceDefault<T>::operator()() const { return T(); }
}
