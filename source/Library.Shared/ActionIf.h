#pragma once
#include "ActionList.h"

namespace FieaGameEngine::Actions {
    class ActionIf final : public ActionList {
        RTTI_DECLARATIONS(ActionIf, ActionList);

    public:
        using Integer = Datum::Integer;

        static SignatureVector Signatures();

        ActionIf();
        ActionIf(const String& name);
        ActionIf(const ActionIf&);
        ActionIf(ActionIf&&) noexcept;
        ActionIf& operator=(const ActionIf&);
        ActionIf& operator=(ActionIf&&) noexcept;
        ~ActionIf();

        void Update(const GameTime& gameTime) override;

        [[nodiscard]] Integer& Condition();
        [[nodiscard]] Integer GetCondition() const;
        void SetCondition(Integer);

        [[nodiscard]] const Datum& GetElseActions() const;
        [[nodiscard]] const Action& GetElseAction(size_type index = size_type(0)) const;

        void Clear() override;
        [[nodiscard]] ScopeUniquePointer Clone() const override;
        [[nodiscard]] std::string ToString() const override;

        void swap(ActionIf&);

    private:
        static const key_type CONDITION_KEY;
        static const key_type ELSE_KEY;

        Integer _condition{0};
        Datum* _else{nullptr};

    };

    void swap(ActionIf& lhs, ActionIf& rhs);

    FACTORY(ActionIf, Scope);
}

#include "ActionIf.inl"
