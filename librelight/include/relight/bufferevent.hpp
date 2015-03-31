//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

#include <relight/evbuffer.hpp>
#include <relight/pointer.hpp>
#include <relight/transport.hpp>

#include <event2/bufferevent.h>
#include <event2/event.h>

namespace relight {

inline Pointer<bufferevent>
make_bufferevent_socket(Pointer<event_base> base, evutil_socket_t sock, int options) {
    return Pointer<bufferevent>{bufferevent_socket_new(base, sock, opts),
            [](bufferevent *p) { if (p) bufferevent_free(p); }
    };
}

class BuffereventTransport : public Transport {

    Pointer<Transport> self;
    Pointer<bufferevent> buffev;
    Pointer<event_base> evbase;

    static void do_read(bufferevent *, void *opaque) {
        auto btp = static_cast<BuffereventTransport *>(opaque);

        // Hold a reference until the end of this function
        Pointer<Transport> transport = btp->self;

        evbuffer *buffer = bufferevent_get_input(transport->buffev);

        ssize_t ctrl = evbuffer_peek(buffer, -1, NULL, NULL, 0);
        if (ctrl <= 0)
            throw std::runtime_error("evbuffer_peek() failed");
        size_t size = static_cast<size_t>(ctrl);
        Pointer<iovec> iov = make_iovec(size);
        iovec *iovp = iov;
        if (evbuffer_peek(buffer, -1, NULL, iovp, size) != ctrl)
            throw std::runtime_error("evbuffer_peek() failed");

        for (size_t i = 0; i < size; i++)
            transport->on_data(iovp[i]->iov_base, iovp[i]->iov_len);
            if (!transport->on_data)
                break;  // Close was called and on_data() was reset
    }

    static void do_write(bufferevent *, void *opaque) {
        auto btp = static_cast<BuffereventTransport *>(opaque);
        btp->on_flush();
    }

    static void do_error(bufferevent *, short what, void *opaque) {
        auto btp = static_cast<BuffereventTransport *>(opaque);
        btp->on_error(new Error(what));
    }

  public:

    static Pointer<Transport>
    make(Pointer<event_base> evbase, evutil_socket_t so) {
        Pointer<BuffereventTransport> transport{new BuffereventTransport};
        transport->buffev = make_bufferevent_socket(evbase, so,
                                BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(transport->buffev, transport->do_read, transport->do_write,
                      transport->do_error, transport.get());
        if (bufferevent_enable(transport->buffev, EV_READ) != 0)
            throw std::runtime_error("bufferevent_enable() failed");

        transport->send = [transport](const void *b, size_t n) {
            if (bufferevent_write(transport->buffev, b, n) != 0)
                throw std::bad_alloc();
        };
        transport->close = [transport]() {
            if (transport->on_destroy)
                transport->on_destroy();

            // Clear the callbacks to remove possible circular references
            transport->on_connect = nullptr;
            transport->on_data = nullptr;
            transport->send = nullptr;
            transport->on_flush = nullptr;
            transport->on_destroy = nullptr;
            transport->on_error = nullptr;

            // Also remove self reference
            transport->self = nullptr;
        };

        // Add self reference to keep this safe
        transport->self = transport;

        return transport;
    }
};
}
