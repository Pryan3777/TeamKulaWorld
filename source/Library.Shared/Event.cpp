#include "pch.h"
#include "Event.h"
#include <cmath>

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    std::unique_ptr<typename Event::map_type> Event::_static_subscriptions = std::make_unique<typename Event::map_type>();

    bool Event::CreateSubscriptionsSingleton() {
        if (_static_subscriptions) {
            return false;
        }

        _static_subscriptions = std::make_unique<map_type>();
        return true;
    }

    std::weak_ptr<typename Event::mapped_type> Event::FindOrCreateSubscribers(IEventArgs::IdType type) {
        auto found = _static_subscriptions->Find(type);

        if (found == _static_subscriptions->end()) {
            found = _static_subscriptions->Insert(std::make_pair(type, std::make_shared<mapped_type>()));
        }

        return found->second;
    }

    bool Event::Subscribe(IEventArgs::IdType type, SubscriberWeakPtr subscriber, std::string name) {
        if (!_static_subscriptions) {
            throw std::logic_error(
                "Event subscriptions have not been initialized, cannot subscribe"s
                + (name.empty() ? ""s : (" to events of type "s + name))
                + "!"s
            );
        }

        auto locked = subscriber.lock();
        if (!locked) {
            return false;
        }

        auto& subscribers = FindOrCreateSubscribers(type).lock()->second;
        return (subscribers.CFind(subscriber, [](const SubscriberWeakPtr& lhs, const SubscriberWeakPtr& rhs){ return lhs.lock() == rhs.lock(); }) == subscribers.cend())
            ? (subscribers.EmplaceBack(locked) != subscribers.end())
            : false;
    }

    bool Event::Unsubscribe(IEventArgs::IdType type, SubscriberWeakPtr subscriber, std::string name) {
        if (!_static_subscriptions) {
            throw std::logic_error(
                "Event subscriptions have not been initialized, cannot unsubscribe"s
                + (name.empty() ? ""s : (" from events of type "s + name))
                + "!"s
            );
        }

        auto locked = subscriber.lock();
        if (!locked) {
            return false;
        }

        auto found = _static_subscriptions->Find(type);
        if (found == _static_subscriptions->end()) {
            return false;
        }

        if (found->second->second.Remove(subscriber, [](const SubscriberWeakPtr& lhs, const SubscriberWeakPtr& rhs) { return lhs.lock() == rhs.lock(); })) {
            auto index = found->second->first;
            found->second->first = std::min(index, index - decltype(index)(1));
            return true;
        }

        return false;
    }

    void Event::ClearSubscriptions(IEventArgs::IdType type, std::string name) {
        if (!_static_subscriptions) {
            throw std::logic_error(
                "Event subscriptions have not been initialized, cannot clear subscription"s
                + (name.empty() ? "s"s : (" events of type "s + name))
                + "!"s
            );
        }

        auto found = _static_subscriptions->Find(type);
        if (found != _static_subscriptions->end()) {
            found->second->second.Clear();
        }
    }

    Event& Event::operator=(const Event& other) {
        if (this != &other) {
            _args = other._args->Clone();
            _subscribers = other._subscribers;
        }

        return *this;
    }

    void Event::Publish() const {
        assert(_args);

        auto subscribersPtr = _subscribers.lock();
        if (!subscribersPtr) {
            throw std::logic_error("Cannot publish event, subscribers pointer has been invalidated!"s);
        }

        auto first = subscribersPtr->first;
        auto size = subscribersPtr->second.Size();
        array_shared_type validSubscribers{size};

        {
            for (auto i = first; i < size; ++i) {
                auto subscriber = subscribersPtr->second.At(i).lock();

                if (subscriber) {
                    validSubscribers.EmplaceBack(std::move(subscriber));
                } else {
                    subscribersPtr->second.RemoveAt(i);
                    --i;
                    --size;
                }
            }
        }

        subscribersPtr->first = std::max(subscribersPtr->first, size);
        {
            for (const auto& valid : validSubscribers) {
                valid->Notify(*_args);
            }
        }
        subscribersPtr->first = std::min(subscribersPtr->first, first);
    }
}
