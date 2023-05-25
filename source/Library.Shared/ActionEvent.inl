#pragma once
#include "ActionEvent.h"

namespace FieaGameEngine::Actions {
    inline ActionEvent::ActionEvent(String subtype, int delay) : ActionEvent{String{}, std::move(subtype), delay} {}
    inline ActionEvent::ActionEvent(const String& name, String subtype, int millisecondDelay)
        : Action{ActionEvent::TypeIdClass(), name}
        , _subtype{std::move(subtype)}
        , _millisecondDelay{millisecondDelay} {}

    inline typename ActionEvent::String& ActionEvent::Subtype() { return _subtype; }
    inline const typename ActionEvent::String& ActionEvent::GetSubtype() const { return _subtype; }
    inline void ActionEvent::SetSubtype(String value) noexcept { _subtype = std::move(value); }
    inline int& ActionEvent::MillisecondDelay() { return _millisecondDelay; }
    inline int ActionEvent::GetMillisecondDelay() const { return _millisecondDelay; }
    inline void ActionEvent::SetMillisecondDelay(int value) noexcept { _millisecondDelay = value; }

    inline typename ActionEvent::ScopeUniquePointer ActionEvent::Clone() const { return std::make_unique<ActionEvent>(*this); }
}
