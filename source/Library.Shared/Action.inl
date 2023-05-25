#pragma once
#include "Action.h"

namespace FieaGameEngine {
    inline SignatureVector Action::Signatures() { return SignatureVector{ Signature{NAME_KEY, Datum::DatumType::String, true, Datum::size_type(1), offsetof(Action, _name)} }; }

    inline Action::Action(IdType idOfSignaturesToAppend, const String& name) : Attributed{idOfSignaturesToAppend}, _name{name} {}
    inline Action::Action(const Action& other) : Attributed{other}, _name{other._name} {}
    inline Action::Action(Action&& other) noexcept : Attributed{std::move(other)}, _name{other._name} { other._name.clear(); }

    inline typename Action::String& Action::Name() { return _name; }
    inline const typename Action::String& Action::GetName() const { return _name; }
    inline void Action::SetName(const String& name) { _name = name; }

    inline void Action::swap(Action& other) { Attributed::swap(other); using std::swap; swap(_name, other._name); }
    inline void swap(Action& lhs, Action& rhs) { lhs.swap(rhs); }
}
