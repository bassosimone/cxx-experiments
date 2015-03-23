//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

#include <relight/error.hpp>
#include <relight/pointer.hpp>

namespace relight {

using namespace relight;

struct Transport {

    std::function<void()> on_connect;

    std::function<void(const void *, size_t)> on_data;

    std::function<void()> on_flush;

    std::function<void(Pointer<Error>)> on_error;

    std::function<void()> on_destroy;

    std::function<void(const void *, size_t)> send;

    std::function<void()> close;
};

Pointer<Transport> make_transport() {
    return Pointer<Transport>{new Transport};
}
}
