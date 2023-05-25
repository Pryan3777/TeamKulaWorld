#include "pch.h"
#include "EventQueue.h"
#include <algorithm>

namespace FieaGameEngine {
    void EventQueue::Enqueue(Event&& event, milli_type delayInMillis, const timepoint_type& now) {
        if (delayInMillis < INSTANT_PUBLISH) {
            event.Publish();
            return;
        }

        Enqueue(std::move(event), now + std::chrono::duration<milli_type, std::chrono::milliseconds::period>(delayInMillis));
    }

    void EventQueue::Enqueue(Event&& event, timepoint_type expiration) {
        if (_isUpdating) {
            _pendingOperations.EmplaceBack(QueueOperation::Enqueue(std::move(event), std::move(expiration)));
            return;
        }

        _queue.EmplaceBack(std::make_pair(std::move(expiration), std::move(event)));
        ResolveNextPendingOperation();
    }

    void EventQueue::RemoveEach(RemovalPredicate predicate) {
        if (_isUpdating) {
            _pendingOperations.EmplaceBack(QueueOperation::RemoveEach(predicate));
            return;
        }

        for (auto i = decltype(_queue)::size_type(0); i < _queue.Size(); ++i) {
            if (predicate(_queue[i].second.Args(), _queue[i].first)) {
                _queue.RemoveAt(i);
                --i;
            }
        }

        ResolveNextPendingOperation();
    }

    void EventQueue::Clear() {
        if (_isUpdating) {
            _pendingOperations.EmplaceBack(QueueOperation::Clear());
            return;
        }

        _queue.Clear();
        ResolveNextPendingOperation();
    }

    void EventQueue::Update(const GameTime& gameTime) {
        if (_isUpdating) {
            return;
        }

        _isUpdating = true;

        auto partition = std::partition(_queue.begin(), _queue.end(), [&gameTime](const array_type::value_type& value) {
            return value.first >= gameTime.CurrentTime();
        });
        for (auto i = partition; i < _queue.end(); ++i) {
            i->second.Publish();
        }
        _queue.Remove(partition, _queue.end());

        _isUpdating = false;
        ResolveNextPendingOperation();
    }

    void EventQueue::ResolveNextPendingOperation() {
        if (_pendingOperations.IsEmpty()) {
            return;
        }

        QueueOperation operation = std::move(_pendingOperations.Front());
        _pendingOperations.PopFront();

        switch (operation._type) {

        case QueueOperation::Type::CLEAR:
            Clear();
            break;

        case QueueOperation::Type::ENQUEUE:
            Enqueue(std::move(operation._event), std::move(operation._eventExpiration));
            break;

        case QueueOperation::Type::REMOVE_EACH:
            RemoveEach(operation._removalPredicate);
            break;

        }
    }

    EventQueue::QueueOperation EventQueue::QueueOperation::Enqueue(Event&& event, timepoint_type&& expiration) {
        QueueOperation operation{};
        operation._type = Type::ENQUEUE;
        operation._event = std::move(event);
        operation._eventExpiration = std::move(expiration);
        return operation;
    }
}
