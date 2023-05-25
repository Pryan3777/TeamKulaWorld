#pragma once
#include "EventSubscriber.h"

namespace FieaGameEngine {
    inline EventSubscriber::EventSubscriber() noexcept : EventSubscriber{DefaultNotify{}} {}
    inline EventSubscriber::EventSubscriber(delegate_type delegate) noexcept : Notify{std::move(delegate)} {}
    inline void EventSubscriber::DefaultNotify::operator()(const IEventArgs&) const {}
}
