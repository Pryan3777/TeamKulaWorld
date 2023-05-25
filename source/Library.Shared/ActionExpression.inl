#pragma once
#include "ActionExpression.h"

namespace FieaGameEngine::Actions {
    inline ActionExpression::ActionExpression(const ActionExpression& other) : Action{other}, _expression{other._expression} {}

    inline const typename ActionExpression::String& ActionExpression::GetReversePolishNotatedExpression() const { return _expression; }
    inline void ActionExpression::SetReversePolishNotatedExpression(const String& expression) { _expression = expression; }
    inline const Datum& ActionExpression::GetLastResult() const { return _lastResult; }

    inline void ActionExpression::Update(const GameTime&) { Evaluate(); }
    inline bool ActionExpression::Evaluate() { return Evaluate(_parent); }

    inline void ActionExpression::Clear() { Action::Clear(); _expression.clear(); _lastResult.Clear(); }
    inline typename ActionExpression::ScopeUniquePointer ActionExpression::Clone() const { return std::make_unique<ActionExpression>(*this); }

    inline void swap(ActionExpression& lhs, ActionExpression& rhs) { lhs.swap(rhs); }
}
