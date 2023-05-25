#pragma once
#include "ActionIf.h"

namespace FieaGameEngine::Actions {
    inline ActionIf::ActionIf() : ActionIf{String{}} {}
    inline ActionIf::ActionIf(const ActionIf& other) : ActionList{other} { _else = &(At(ELSE_KEY)); }
    inline ActionIf::ActionIf(ActionIf&& other) noexcept : ActionList{std::move(other)} { _else = &(At(ELSE_KEY)); }
    inline ActionIf& ActionIf::operator=(const ActionIf& other) { ActionIf copy{other}; swap(copy); return *this; }
    inline ActionIf::~ActionIf() { _else = nullptr; }

    inline void ActionIf::Update(const GameTime& gameTime) {
        assert(_else != nullptr);
        UpdateActions(gameTime, ((CSearch(CONDITION_KEY)->CFrontInteger()) == 0) ? (*_else) : Actions());
    }

    inline typename ActionIf::Integer& ActionIf::Condition() { return _condition; }
    inline typename ActionIf::Integer ActionIf::GetCondition() const { return _condition; }
    inline void ActionIf::SetCondition(Integer condition) { _condition = condition; }

    inline const Datum& ActionIf::GetElseActions() const { assert(_else != nullptr); return *_else; }
    inline const Action& ActionIf::GetElseAction(size_type index) const {
        assert(_else != nullptr);
        const Scope& action = _else->CGetTableElement(index);
        assert(action.Is(Action::TypeIdClass()));
        return static_cast<const Action&>(action);
    }

    inline void ActionIf::Clear() { ActionList::Clear(); assert(_else != nullptr); _else->Clear(); }
    inline typename ActionIf::ScopeUniquePointer ActionIf::Clone() const { return std::make_unique<ActionIf>(*this); }

    inline void ActionIf::swap(ActionIf& other) { ActionList::swap(other); _else = &(At(ELSE_KEY)); other._else = &(other.At(ELSE_KEY)); }
    inline void swap(ActionIf& lhs, ActionIf& rhs) { lhs.swap(rhs); }
}
