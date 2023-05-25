#pragma once
#include <memory>
#include "EventSubscriber.h"
#include "HashMap.h"
#include "IEventArgs.h"
#include "Vector.h"

namespace FieaGameEngine {
    class Event final {

    public:
        using SubscriberWeakPtr = std::weak_ptr<EventSubscriber>;

        static bool CreateSubscriptionsSingleton();
        static void DestroySubscriptionsSingleton();
        template <typename TEventArgs> static bool Subscribe(SubscriberWeakPtr subscriber);
        template <typename TEventArgs> static bool Unsubscribe(SubscriberWeakPtr subscriber);
        template <typename TEventArgs> static void ClearSubscriptions();
        static bool Subscribe(IEventArgs::IdType type, SubscriberWeakPtr subscriber);
        static bool Unsubscribe(IEventArgs::IdType type, SubscriberWeakPtr subscriber);
        static void ClearSubscriptions(IEventArgs::IdType type);
        static void Publish(std::unique_ptr<IEventArgs> args);
        template <typename TEventArgs, typename... CtorArgs> static void Publish(CtorArgs&&... args);

        Event(std::unique_ptr<IEventArgs>);
        Event(const Event&);
        Event(Event&&) noexcept = default;
        Event& operator=(const Event&);
        Event& operator=(Event&&) noexcept = default;
        ~Event() = default;

        void Publish() const;
        const IEventArgs& Args() const;

    private:
        using array_weak_type = Vector<SubscriberWeakPtr>;
        using array_shared_type = Vector<std::shared_ptr<EventSubscriber>>;
        using mapped_type = std::pair<array_weak_type::size_type, array_weak_type>;
        using map_type = HashMap<IEventArgs::IdType, std::shared_ptr<mapped_type>>;

        static std::weak_ptr<mapped_type> FindOrCreateSubscribers(IEventArgs::IdType type);
        static bool Subscribe(IEventArgs::IdType type, SubscriberWeakPtr subscriber, std::string name);
        static bool Unsubscribe(IEventArgs::IdType type, SubscriberWeakPtr subscriber, std::string name);
        static void ClearSubscriptions(IEventArgs::IdType type, std::string name);

        static std::unique_ptr<map_type> _static_subscriptions;

        std::unique_ptr<IEventArgs> _args;
        std::weak_ptr<mapped_type> _subscribers;

    };
}

#include "Event.inl"
