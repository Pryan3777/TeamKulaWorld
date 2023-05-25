#pragma once
#include "EventQueue.h"

namespace FieaGameEngine {
    inline void EventQueue::Enqueue(const IEventArgs& args, milli_type delayInMillis, const timepoint_type& now) { Enqueue(args.Clone(), delayInMillis, now); }
    template <typename TEventArgs, typename... Args> inline void EventQueue::Enqueue(milli_type delayInMillis, const timepoint_type& now, Args&&... args) {
        Enqueue(std::make_unique<TEventArgs>(std::forward<Args>(args)...), delayInMillis, now);
    }
    inline void EventQueue::Enqueue(std::unique_ptr<IEventArgs> args, milli_type delayInMillis, const timepoint_type& now) { Enqueue(Event{std::move(args)}, delayInMillis, now); }
    inline void EventQueue::Enqueue(const Event& event, milli_type delayInMillis, const timepoint_type& now) { Enqueue(Event{event}, delayInMillis, now); }

    inline EventQueue::QueueOperation::QueueOperation()
        : _type{Type::CLEAR}
        , _event{std::make_unique<InvalidEventArgs>()}
        , _eventExpiration{}
        , _removalPredicate{[](const IEventArgs&, const timepoint_type&){ return false; }} {}
    inline EventQueue::QueueOperation EventQueue::QueueOperation::Clear() { QueueOperation op{}; return op; }
    inline EventQueue::QueueOperation EventQueue::QueueOperation::RemoveEach(RemovalPredicate pred) { QueueOperation op{}; op._type = Type::REMOVE_EACH; op._removalPredicate = pred; return op; }

    inline bool EventQueue::IsUpdating() const { return _isUpdating; }
    inline bool EventQueue::IsEmpty() const { return _queue.IsEmpty(); }
    inline std::size_t EventQueue::Size() const { return _queue.Size(); }
}
