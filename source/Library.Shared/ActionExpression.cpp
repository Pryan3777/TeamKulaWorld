#include "pch.h"
#include "ActionExpression.h"
#include <regex>
#include "GameplayState.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine::Actions {
    RTTI_DEFINITIONS(ActionExpression);

    const typename ActionExpression::key_type ActionExpression::EXPRESSION_KEY = "Expression"s;

    SignatureVector ActionExpression::Signatures() { return SignatureVector{Signature{EXPRESSION_KEY, DatumType::String, true, size_type(1), offsetof(ActionExpression, _expression)}}; }

    ActionExpression::ActionExpression() : Action{ActionExpression::TypeIdClass(), String{}} {}

    ActionExpression& ActionExpression::operator=(const ActionExpression& other) {
        if (this != &other) {
            Attributed::operator=(other);
            _expression = other._expression;
            _lastResult.Clear();
        }

        return *this;
    }

    inline bool ActionExpression::Evaluate(Scope* context) {
        return _expression.empty()
            ? false 
            : (_lastResult = GameplayState::Singleton().RpnEvaluator().Evaluate(_expression, !!context ? *context : *this)).IsTruthy();
    }

    std::string ActionExpression::ToString() const {
        std::stringstream tostring{};
        tostring << TypeNameInstance() << ": { \""s << NAME_KEY << "\": "s << GetName() << ", \""s << EXPRESSION_KEY << "\": \""s << _expression << "\" }"s;
        return tostring.str();
    }

    void ActionExpression::swap(ActionExpression& other) {
        Action::swap(other);

        using std::swap;

        swap(_expression, other._expression);
        swap(_lastResult, other._lastResult);
    }
}
