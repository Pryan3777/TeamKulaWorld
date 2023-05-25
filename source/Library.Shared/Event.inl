#pragma once
#include "Event.h"
#include <cassert>

namespace FieaGameEngine {
    inline void Event::DestroySubscriptionsSingleton() { _static_subscriptions.reset(); }
    template <typename TEventArgs> inline bool Event::Subscribe(SubscriberWeakPtr s) { return s.expired() ? false : Event::Subscribe(TEventArgs::TypeIdClass(), std::move(s), TEventArgs::TypeNameClass()); }
    template <typename TEventArgs> inline bool Event::Unsubscribe(SubscriberWeakPtr s) { return s.expired() ? false : Event::Unsubscribe(TEventArgs::TypeIdClass(), std::move(s), TEventArgs::TypeNameClass()); }
    template <typename TEventArgs> inline void Event::ClearSubscriptions() { Event::ClearSubscriptions(TEventArgs::TypeIdClass(), TEventArgs::TypeNameClass()); }
    inline bool Event::Subscribe(IEventArgs::IdType t, SubscriberWeakPtr s) { return s.expired() ? false : Event::Subscribe(t, std::move(s), std::string{}); }
    inline bool Event::Unsubscribe(IEventArgs::IdType t, SubscriberWeakPtr s) { return s.expired() ? false : Event::Unsubscribe(t, std::move(s), std::string{}); }
    inline void Event::ClearSubscriptions(IEventArgs::IdType t) { Event::ClearSubscriptions(t, std::string{}); }

    template <typename TEventArgs, typename... CtorArgs> inline void Event::Publish(CtorArgs&&... args) { Publish(std::make_unique<TEventArgs>(std::forward(args)...)); }
    inline void Event::Publish(std::unique_ptr<IEventArgs> args) { Event ev{std::move(args)}; ev.Publish(); }

    inline Event::Event(std::unique_ptr<IEventArgs> args) : _args{std::move(args)} { assert(_args); _subscribers = FindOrCreateSubscribers(_args->TypeIdInstance()); }
    inline Event::Event(const Event& other) : _args{other._args->Clone()}, _subscribers{other._subscribers} {}

    inline const IEventArgs& Event::Args() const { assert(_args); return *_args; }
}
