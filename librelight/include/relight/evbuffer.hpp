//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

#include <relight/pointer.hpp>

#include <event2/buffer.h>

namespace relight {

inline Pointer<iovec> make_iovec(size_t s) {
    if (s <= 0)
        throw std::runtime_error("bad iovec size");
    return Pointer<iovec>{new iovec[s]};
}

inline Pointer<evbuffer> make_evbuffer() {
    return Pointer<evbuffer>{evbuffer_new(), [](evbuffer *p) {
        if (p != nullptr)
            evbuffer_free(b);
    }};
}

}
