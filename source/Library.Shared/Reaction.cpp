#include "pch.h"
#include "Reaction.h"
#include "Event.h"
#include "GameplayState.h"

namespace FieaGameEngine {
    RTTI_DEFINITIONS(Reaction);

    Reaction& Reaction::operator=(const Reaction& other) {
        if (this != &other) {
            ActionList::operator=(other);
            _subscriber = std::make_shared<EventSubscriber>(*(other._subscriber));
        }

        return *this;
    }

    void Reaction::SubscribeToEvents(std::initializer_list<IEventArgs::IdType> subscriptions) {
        for (const auto& subscription : subscriptions) {
            Event::Subscribe(subscription, _subscriber);
        }
    }

    void Reaction::UnsubscribeFromEvents(std::initializer_list<IEventArgs::IdType> subscriptions) {
        for (const auto& subscription : subscriptions) {
            Event::Unsubscribe(subscription, _subscriber);
        }
    }
}
