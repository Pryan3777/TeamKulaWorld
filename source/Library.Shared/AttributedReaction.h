#pragma once
#include "Reaction.h"
#include "Stack.h"

namespace FieaGameEngine {
    class AttributedReaction : public Reaction {
        RTTI_DECLARATIONS(AttributedReaction, Reaction);

    public:
        static SignatureVector Signatures();

        AttributedReaction();
        explicit AttributedReaction(const String& name);
        AttributedReaction(const AttributedReaction&);
        AttributedReaction(AttributedReaction&&) noexcept;
        AttributedReaction& operator=(const AttributedReaction&);
        AttributedReaction& operator=(AttributedReaction&&) noexcept;
        virtual ~AttributedReaction();

        [[nodiscard]] bool IsReactingTo(const IEventArgs&) const override;

        [[nodiscard]] const Datum& GetReactsToSubtypeRegexes() const;
        [[nodiscard]] const String& GetReactsToSubtypeRegex(size_type index = size_type(0)) const;

        [[nodiscard]] ScopeUniquePointer Clone() const;

    protected:
        static const key_type REACTS_TO_SUBTYPE_REGEX_KEY;

        AttributedReaction(IdType idOfSignaturesToAppend, const String& name);

        virtual void ReactBeforeActionList(const IEventArgs&) override;
        virtual void ReactAfterActionList(const IEventArgs&) override;

        [[nodiscard]] Datum& ReactsToSubtypeRegexes();

    private:
        Datum* _reactsToSubtypeRegexes{nullptr};
        Stack<const Scope*> _pushedArguments{};

    };

    FACTORY(AttributedReaction, Scope);
}

#include "AttributedReaction.inl"
