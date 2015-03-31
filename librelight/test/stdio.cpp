//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#include <relight/bufferevent.hpp>
#include <relight/loop.hpp>

#include <iostream>

using namespace relight;

int main() {
    loop([](CPointer<event_base> base, CPointer<evdns_base>) {
        Pointer<Transport> t = bufferevent_attach(base, 0);
        t->on_data = [t](const void *d, size_t n) {
            std::cerr << "got data\n";
            t->send(d, n);
        };
        t->on_flush = []() {
            std::cerr << "flushed\n";
        };
        t->on_error = [t](Pointer<Error> err) {
            std::cerr << "error: " << int(err) << "\n";
            t->close();
        };
        t->on_destroy = [base]() {
            event_base_loopbreak(base);
        };
    });
}
