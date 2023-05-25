#pragma once
#include "GameplayState.h"
#include "Reaction.h"

namespace FieaGameEngine {
    inline SignatureVector Reaction::Signatures() { return SignatureVector{}; }

    inline Reaction::Reaction(const String& name, std::initializer_list<IEventArgs::IdType> subscriptions)
        : Actions::ActionList{Reaction::TypeIdClass(), name}
        , _subscriber{CreateSubscriber()}
    {
        SubscribeToEvents(std::move(subscriptions));
    }
    inline Reaction::Reaction() : Reaction{String{}, {}} {}
    inline Reaction::Reaction(std::initializer_list<IEventArgs::IdType> subscriptions) : Reaction{String{}, std::move(subscriptions)} {}
    inline Reaction::Reaction(IdType idOfSignaturesToAppend, const String& name, std::initializer_list<IEventArgs::IdType> subscriptions)
        : Actions::ActionList{idOfSignaturesToAppend, name}
        , _subscriber{CreateSubscriber()}
    {
        SubscribeToEvents(std::move(subscriptions));
    }
    inline Reaction::Reaction(const Reaction& other) : Actions::ActionList{other}, _subscriber{std::make_shared<EventSubscriber>(*(other._subscriber))} {}

    inline bool Reaction::IsReactingTo(const IEventArgs&) const { return true; }
    inline void Reaction::ReactBeforeActionList(const IEventArgs&) {}
    inline void Reaction::ReactAfterActionList(const IEventArgs&) {}

    inline std::shared_ptr<EventSubscriber> Reaction::CreateSubscriber() const {
        auto* ptr = const_cast<Reaction*>(this);
        return std::make_shared<EventSubscriber>([ptr](const IEventArgs& args) {
            if (ptr->IsReactingTo(args)) {
                ptr->ReactBeforeActionList(args);
                ptr->Update(GameplayState::Singleton().GetTime());
                ptr->ReactAfterActionList(args);
            }
        });
    }

    inline typename Reaction::ScopeUniquePointer Reaction::Clone() const { return std::make_unique<Reaction>(*this); }
}
