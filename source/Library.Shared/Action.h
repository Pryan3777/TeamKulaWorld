#pragma once
#include "Attributed.h"
#include "AttributedSignatureRegistry.h"
#include "GameTime.h"

namespace FieaGameEngine {
    class Action : public Attributed {
        RTTI_DECLARATIONS(Action, Attributed);

    public:
        static SignatureVector Signatures();

        Action(const Action&);
        Action(Action&&) noexcept;
        Action& operator=(const Action&) = default;
        Action& operator=(Action&&) noexcept;
        virtual ~Action() = default;

        virtual void Update(const GameTime& gameTime) = 0;

        virtual Datum* Search(const key_type& key, Scope*& outputContainingScope) override;

        String& Name();
        [[nodiscard]] const String& GetName() const;
        void SetName(const String& name);

        void swap(Action&);

    protected:
        static const key_type NAME_KEY;

        Action(IdType idOfSignaturesToAppend, const String& name);

    private:
        String _name{};

    };

    void swap(Action& lhs, Action& rhs);
}

#include "Action.inl"
