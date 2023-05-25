#pragma once
#include <functional>
#include "IEventArgs.h"

namespace FieaGameEngine {
    class EventSubscriber final {

    public:
        using delegate_type = std::function<void(const IEventArgs&)>;

        EventSubscriber() noexcept;
        EventSubscriber(delegate_type) noexcept;

        delegate_type Notify;

    private:
        struct DefaultNotify final {
            void operator()(const IEventArgs&) const;
        };

    };
}

#include "EventSubscriber.inl"
