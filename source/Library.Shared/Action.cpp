#include "pch.h"
#include "Action.h"
#include "GameplayState.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    RTTI_DEFINITIONS(Action);

    const typename Action::key_type Action::NAME_KEY = "Name"s;

    Action& Action::operator=(Action&& other) noexcept {
        if (this != &other) {
            Attributed::operator=(std::move(other));
            _name = std::move(other._name);
            other._name.clear();
        }

        return *this;
    }

    Datum* Action::Search(const key_type& key, Scope*& outputContainingScope) {
        auto found = end();
        auto& stack = GameplayState::Singleton().ActionArgumentStack();

        if (!(stack.IsEmpty())) {
            outputContainingScope = &(*(stack.Top()));
            found = outputContainingScope->Find(key);
        }

        return (found == end()) ? Attributed::Search(key, outputContainingScope) : &(found->second);
    }
}
