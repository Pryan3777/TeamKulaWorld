#include "pch.h"
#include "HeapedIntEventArgs.h"

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(HeapedIntEventArgs);

    HeapedIntEventArgs& HeapedIntEventArgs::operator=(HeapedIntEventArgs&& other) noexcept {
        if (this != &other) {
            delete _data;
            _data = other._data;
            other._data = nullptr;
        }

        return *this;
    }
}
