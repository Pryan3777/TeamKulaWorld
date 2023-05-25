#include "pch.h"
#include "Bar.h"
#include <string>

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(Bar);

    Bar::Bar(number_type number) : Scope(), _number{new number_type{number}} {}

    Bar::Bar(const Bar& other) : Scope(other), _number{new number_type{*(other._number)}} {}

    Bar::Bar(Bar&& other) noexcept : Scope(std::move(other)), _number{other._number} { other._number = nullptr; }

    Bar& Bar::operator=(const Bar& other) {
        if (this != &other) {
            if (_number == nullptr){
                _number = new number_type{*(other._number)};
            } else {
                *_number = *(other._number);
            }
        }

        return *this;
    }

    Bar& Bar::operator=(Bar&& other) noexcept {
        if (this != &other) {
            delete _number;
            _number = other._number;
            other._number = nullptr;
        }

        return *this;
    }

    Bar::~Bar() { delete _number; }

    bool Bar::operator==(const Bar& other) const {
        return (*_number == *(other._number)) && (FieaGameEngine::Scope::operator==(*(other.As<FieaGameEngine::Scope>())));
    }

    std::string Bar::ToString() const {
        using namespace std::literals::string_literals;

        return "Number: "s + std::to_string(*_number) + ", " + FieaGameEngine::Scope::ToString();
    }
}
