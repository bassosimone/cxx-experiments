//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

#include <relight/pointer.hpp>

#include <functional>

#include <event2/dns.h>
#include <event2/event.h>

namespace relight {

void loop(std::function<void(WeakPointer<event_base>,
          WeakPointer<evdns_base>)> setup) {

    std::shared_ptr<event_base> base{event_base_new(), [](event_base *b) {
        if (b) {
            event_base_free(b);
        }
    }};
    if (!base)
        throw std::bad_alloc();

    std::shared_ptr<evdns_base> dns{evdns_base_new(base.get(), 1),
                                    [](evdns_base *b) {
        if (b) {
            evdns_base_free(b, 1);
        }
    }};
    if (!dns)
        throw std::bad_alloc();

    setup(WeakPointer<event_base>{base.get()},
          WeakPointer<evdns_base>{dns.get()});

    if (event_base_dispatch(base.get()) != 0)
        throw std::runtime_error("event_base_dispatch() failed");
}
}
