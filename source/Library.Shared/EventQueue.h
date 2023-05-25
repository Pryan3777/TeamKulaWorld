#pragma once
#include <cstdint>
#include "Event.h"
#include "GameTime.h"
#include "Vector.h"

namespace FieaGameEngine {
    class EventQueue final {

    public:
        using timepoint_type = std::chrono::high_resolution_clock::time_point;
        using milli_type = std::chrono::milliseconds::rep;
        using RemovalPredicate = std::function<bool(const IEventArgs& args, const timepoint_type& expiration)>;

        void Enqueue(const IEventArgs& args, milli_type delayInMillis, const timepoint_type& now);
        template <typename TEventArgs, typename... Args> void Enqueue(milli_type delayInMillis, const timepoint_type& now, Args&&... args);
        void Enqueue(std::unique_ptr<IEventArgs> args, milli_type delayInMillis, const timepoint_type& now);
        void Enqueue(const Event& event, milli_type delayInMillis, const timepoint_type& now);
        void Enqueue(Event&& event, milli_type delayInMillis, const timepoint_type& now);
        void RemoveEach(RemovalPredicate predicate);
        void Clear();

        void Update(const GameTime& gameTime);

        [[nodiscard]] bool IsUpdating() const;
        [[nodiscard]] bool IsEmpty() const;
        [[nodiscard]] std::size_t Size() const;

        EventQueue() = default;
        EventQueue(const EventQueue&) = delete;
        EventQueue(EventQueue&&) noexcept = default;
        EventQueue& operator=(const EventQueue&) = delete;
        EventQueue& operator=(EventQueue&&) noexcept = default;
        ~EventQueue() = default;

    private:
        using array_type = Vector<std::pair<timepoint_type, Event>>;

        static constexpr milli_type INSTANT_PUBLISH = 1L;

        struct QueueOperation final {
            enum class Type : std::uint8_t {
                CLEAR,
                ENQUEUE,
                REMOVE_EACH
            };

            static QueueOperation Clear();
            static QueueOperation Enqueue(Event&& event, timepoint_type&& expiration);
            static QueueOperation RemoveEach(RemovalPredicate predicate);

            QueueOperation();
            QueueOperation(const QueueOperation&) = delete;
            QueueOperation(QueueOperation&&) noexcept = default;
            QueueOperation& operator=(const QueueOperation&) = delete;
            QueueOperation& operator=(QueueOperation&&) noexcept = default;
            ~QueueOperation() = default;

            Type _type;
            RemovalPredicate _removalPredicate;
            Event _event;
            timepoint_type _eventExpiration;
        };

        void Enqueue(Event&& event, timepoint_type expiration);
        void ResolveNextPendingOperation();

        array_type _queue{};
        bool _isUpdating{false};
        SList<QueueOperation> _pendingOperations{};

    };
}

#include "EventQueue.inl"
