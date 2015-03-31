//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

#include <relight/pointer.hpp>

#include <event2/event.h>

namespace relight {

inline Pointer<event_base> make_event_base() {
    return Pointer<event_base>{event_base_new(), [](event_base *p) {
        if (p != nullptr)
            event_base_free(p);
    }}
}

}
