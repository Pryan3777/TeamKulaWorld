#include "pch.h"
#include "ParseCoordinator.h"

namespace FieaGameEngine {
    RTTI_DEFINITIONS(ParseCoordinator::Wrapper);

    bool ParseCoordinator::Wrapper::DecrementDepth() {
        if (_depth > depth_type(0)) {
            --_depth;
            return true;
        }

        return false;
    }

    ParseCoordinator::ParseCoordinator(WrapperSharedPointer wrapper) : _wrapper{std::move(wrapper)} {
        if (!_wrapper) {
            using namespace std::literals::string_literals;

            throw std::invalid_argument("Cannot initialize coordinator with null wrapper pointer."s);
        }

        _wrapper->_coordinator = this;
    }

    ParseCoordinator& ParseCoordinator::operator=(ParseCoordinator&& other) noexcept {
        if (this != &other) {
            _wrapper = std::move(other._wrapper);
        }

        return *this;
    }

    void ParseCoordinator::SetWrapper(WrapperSharedPointer wrapper) {
        if (!wrapper) {
            using namespace std::literals::string_literals;

            throw std::invalid_argument("Cannot set null wrapper pointer."s);
        }

        if (_wrapper.get() != wrapper.get()) {
            _wrapper = std::move(wrapper);
        }

        _wrapper->_coordinator = this;
    }
}
