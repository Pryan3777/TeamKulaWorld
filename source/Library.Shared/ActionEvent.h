#pragma once
#include "Action.h"

namespace FieaGameEngine::Actions {
    class ActionEvent final : public Action {
        RTTI_DECLARATIONS(ActionEvent, Action);

    public:
        static SignatureVector Signatures();

        explicit ActionEvent(String subtype, int millisecondDelay = 0);
        ActionEvent(const String& name, String subtype, int millisecondDelay = 0);

        void Update(const GameTime& gameTime) override;

        String& Subtype();
        [[nodiscard]] const String& GetSubtype() const;
        void SetSubtype(String) noexcept;

        int& MillisecondDelay();
        [[nodiscard]] int GetMillisecondDelay() const;
        void SetMillisecondDelay(int) noexcept;

        [[nodiscard]] ScopeUniquePointer Clone() const override;

    private:
        static const key_type SUBTYPE_KEY;
        static const key_type MILLI_DELAY_KEY;

        String _subtype;
        int _millisecondDelay;

    };

    FACTORY(ActionEvent, Scope, typename ActionEvent::String{});
}

#include "ActionEvent.inl"
