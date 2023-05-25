#pragma once
#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine::Actions {
    class ActionIncrement final : public Action {
        RTTI_DECLARATIONS(ActionIncrement, Action);

    public:
        using Integer = Datum::Integer;

        static SignatureVector Signatures();

        ActionIncrement();
        ActionIncrement(const ActionIncrement&) = default;
        ActionIncrement(ActionIncrement&&) noexcept = default;
        ActionIncrement& operator=(const ActionIncrement&) = default;
        ActionIncrement& operator=(ActionIncrement&&) noexcept = default;
        ~ActionIncrement() = default;

        void Update(const GameTime& gameTime) override;

        Integer& Current();
        Integer& Step();

        Integer GetCurrent() const;
        Integer GetStep() const;

        [[nodiscard]] virtual ScopeUniquePointer Clone() const override;

    private:
        static const key_type CURRENT_KEY;
        static const key_type STEP_KEY;

        Integer _current{0};
        Integer _step{1};

    };

    FACTORY(ActionIncrement, Scope);
}

#include "ActionIncrement.inl"
