//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

#include <relight/error.hpp>
#include <relight/method.hpp>
#include <relight/pointer.hpp>

namespace relight {

using namespace relight;

struct Transport {

    Method<void()> on_connect;

    Method<void(const void *, size_t)> on_data;

    Method<void()> on_flush;

    Method<void(Pointer<Error>)> on_error;

    Method<void()> on_destroy;

    Method<void(const void *, size_t)> send;

    Method<void()> close;
};

Pointer<Transport> make_transport() {
    return Pointer<Transport>{new Transport};
}
}
