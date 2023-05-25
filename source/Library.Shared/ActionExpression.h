#pragma once
#include "Action.h"

namespace FieaGameEngine::Actions {
    class ActionExpression final : public Action {
        RTTI_DECLARATIONS(ActionExpression, Action);

    public:
        static SignatureVector Signatures();

        ActionExpression();
        ActionExpression(const ActionExpression&);
        ActionExpression(ActionExpression&&) noexcept = default;
        ActionExpression& operator=(const ActionExpression&);
        ActionExpression& operator=(ActionExpression&&) noexcept = default;

        [[nodiscard]] const String& GetReversePolishNotatedExpression() const;
        void SetReversePolishNotatedExpression(const String&);

        [[nodiscard]] const Datum& GetLastResult() const;

        void Update(const GameTime&) override;
        bool Evaluate(Scope* context);
        bool Evaluate();

        void Clear() override;
        [[nodiscard]] ScopeUniquePointer Clone() const override;
        [[nodiscard]] std::string ToString() const override;

        void swap(ActionExpression&);

    private:
        static const key_type EXPRESSION_KEY;

        String _expression{};
        Datum _lastResult{};

    };

    void swap(ActionExpression& lhs, ActionExpression& rhs);

    FACTORY(ActionExpression, Scope);
}

#include "ActionExpression.inl"
