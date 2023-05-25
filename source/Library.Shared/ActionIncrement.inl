#pragma once
#include "ActionIncrement.h"

namespace FieaGameEngine::Actions {
    inline void ActionIncrement::Update(const GameTime&) { _current += CSearch(STEP_KEY)->CFrontInteger(); }
    inline typename ActionIncrement::Integer& ActionIncrement::Current() { return _current; }
    inline typename ActionIncrement::Integer& ActionIncrement::Step() { return _step; }
    inline typename ActionIncrement::Integer ActionIncrement::GetCurrent() const { return _current; }
    inline typename ActionIncrement::Integer ActionIncrement::GetStep() const { return _step; }
    inline typename ActionIncrement::ScopeUniquePointer ActionIncrement::Clone() const { return std::make_unique<ActionIncrement>(*this); }
}
