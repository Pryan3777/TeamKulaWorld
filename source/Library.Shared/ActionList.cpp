#include "pch.h"
#include "ActionList.h"
#include <sstream>

using namespace std::literals::string_literals;

namespace FieaGameEngine::Actions {
    RTTI_DEFINITIONS(ActionList);

    const typename ActionList::key_type ActionList::ACTIONS_KEY = "Actions"s;

    ActionList::ActionList(const String& name) : Action{ActionList::TypeIdClass(), name} {
        _actions = &(At(ACTIONS_KEY));
    }

    ActionList& ActionList::operator=(ActionList&& other) noexcept {
        if (this != &other) {
            Action::operator=(std::move(other));
            _actions = &(At(ACTIONS_KEY));
        }

        return *this;
    }

    void ActionList::UpdateActions(const GameTime& gameTime, Datum& actions) {
        for (auto i = size_type(0); i < actions.Size(); ++i) {
            assert(actions.GetTableElement(i).Is(Action::TypeIdClass()));
            auto* action = static_cast<Action*>(&actions.GetTableElement(i));

            assert(action != nullptr);
            action->Update(gameTime);
        }
    }

    std::string ActionList::ToString() const {
        assert(_actions != nullptr);
        std::stringstream tostring{};
        tostring << TypeNameInstance() << ": { \""s << NAME_KEY << "\": "s << GetName() << ", number of actions: "s << _actions->Size() << " }"s;
        return tostring.str();
    }
}
