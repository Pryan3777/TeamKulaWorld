#pragma once
#include <memory>
#include "ActionList.h"
#include "EventSubscriber.h"

namespace FieaGameEngine {
    class Reaction : public Actions::ActionList {
        RTTI_DECLARATIONS(Reaction, Actions::ActionList);

    public:
        static SignatureVector Signatures();

        Reaction();
        explicit Reaction(std::initializer_list<IEventArgs::IdType> subscriptions);
        explicit Reaction(const String& name, std::initializer_list<IEventArgs::IdType> subscriptions);
        Reaction(const Reaction&);
        Reaction(Reaction&&) noexcept = default;
        Reaction& operator=(const Reaction&);
        Reaction& operator=(Reaction&&) noexcept = default;
        virtual ~Reaction() = default;

        [[nodiscard]] virtual bool IsReactingTo(const IEventArgs&) const;

        void SubscribeToEvents(std::initializer_list<IEventArgs::IdType> subscriptions);
        void UnsubscribeFromEvents(std::initializer_list<IEventArgs::IdType> subscriptions);

        [[nodiscard]] virtual ScopeUniquePointer Clone() const override;

    protected:
        Reaction(IdType idOfSignaturesToAppend, const String& name, std::initializer_list<IEventArgs::IdType> subscriptions);

        virtual void ReactBeforeActionList(const IEventArgs&);
        virtual void ReactAfterActionList(const IEventArgs&);

    private:
        std::shared_ptr<EventSubscriber> _subscriber;

        std::shared_ptr<EventSubscriber> CreateSubscriber() const;

    };

    FACTORY(Reaction, Scope);
}

#include "Reaction.inl"
