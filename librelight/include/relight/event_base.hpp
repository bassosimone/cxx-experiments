//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

#include <memory>
#include <functional>

#include <event2/event.h>

namespace relight {

class EventBase {

    std::shared_ptr<event_base> evbase{
        event_base_new(), [](event_base *b) {
            if (b != nullptr)
                event_base_free();
        }
    };

  public:
    operator event_base *() {
        auto ptr = evbase.get();
        if (ptr == nullptr)
            throw std::runtime_error("null pointer");
        return ptr;
    }
};
}
