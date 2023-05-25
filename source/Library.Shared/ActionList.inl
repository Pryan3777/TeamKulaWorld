#pragma once
#include "ActionList.h"

namespace FieaGameEngine::Actions {
    inline SignatureVector ActionList::Signatures() { return SignatureVector{ Signature{ACTIONS_KEY, DatumType::InternalTable, false, size_type(0), std::size_t(0)} }; }

    inline ActionList::ActionList() : ActionList{String{}} {}
    inline ActionList::ActionList(IdType idOfSignaturesToAppend, const String& name) : Action{idOfSignaturesToAppend, name} { _actions = &(At(ACTIONS_KEY)); }
    inline ActionList::ActionList(const ActionList& other) : Action{other} { _actions = &(At(ACTIONS_KEY)); }
    inline ActionList::ActionList(ActionList&& other) noexcept : Action{std::move(other)} { _actions = &(At(ACTIONS_KEY)); }
    inline ActionList& ActionList::operator=(const ActionList& other) { ActionList copy{other}; swap(copy); return *this; }
    inline ActionList::~ActionList() { _actions = nullptr; }

    inline void ActionList::UpdateSelf(const GameTime&) {}
    inline void ActionList::Update(const GameTime& gameTime) { UpdateSelf(gameTime); UpdateActions(gameTime, Actions()); }

    inline Datum& ActionList::Actions() { assert(_actions != nullptr); return *_actions; }
    inline const Datum& ActionList::GetActions() const { assert(_actions != nullptr); return *_actions; }
    inline const Action& ActionList::GetAction(size_type index) const { return const_cast<ActionList*>(this)->ActionRef(index); }
    inline Action& ActionList::ActionRef(size_type index) {
        assert(_actions != nullptr);
        Scope& action = _actions->GetTableElement(index);
        assert(action.Is(Action::TypeIdClass()));
        return static_cast<Action&>(action);
    }

    inline void ActionList::Clear() { Attributed::Clear(); assert(_actions != nullptr); _actions->Clear(); }
    inline typename ActionList::ScopeUniquePointer ActionList::Clone() const { return std::make_unique<ActionList>(*this); }

    inline void ActionList::swap(ActionList& other) { Action::swap(other); _actions = &(At(ACTIONS_KEY)); other._actions = &(other.At(ACTIONS_KEY)); }
    inline void swap(ActionList& lhs, ActionList& rhs) { lhs.swap(rhs); }
}
