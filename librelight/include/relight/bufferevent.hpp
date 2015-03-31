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

Pointer<Transport> bufferevent_attach(CPointer<event_base> base,
                                      evutil_socket_t sockfd) {

    struct Context {
        Pointer<Transport> transport;
        CPointer<bufferevent> bufev;

        static void do_read(bufferevent *, void *opaque) {

            Pointer<Transport> transport;
            EvBuffer buffer;

            {
                CPointer<Context> ctx = static_cast<Context *>(opaque);
                CPointer<evbuffer> src = bufferevent_get_input(ctx->bufev);
                if (evbuffer_add_buffer(buffer, src) != 0)
                    throw std::runtime_error("evbuffer_add_buffer() failed");
                transport = ctx->transport;
            }

            ssize_t ctrl = evbuffer_peek(buffer, -1, NULL, NULL, 0);
            if (ctrl <= 0)
                throw std::runtime_error("evbuffer_peek() failed");
            size_t size = static_cast<size_t>(ctrl);
            EvBufferIovec iovec{size};
            if (evbuffer_peek(buffer, -1, NULL, iovec, size) != ctrl)
                throw std::runtime_error("evbuffer_peek() failed");

            for (size_t i = 0; i < size && transport->on_data; i++)
                transport->on_data(iovec[i]->iov_base, iovec[i]->iov_len);
        }

        static void do_write(bufferevent *, void *opaque) {
            CPointer<Context> ctx = static_cast<Context *>(opaque);
            ctx->transport->on_flush();
        }

        static void do_error(bufferevent *, short what, void *opaque) {
            CPointer<Context> ctx = static_cast<Context *>(opaque);
            Pointer<Error> err = new Error(what);
            ctx->transport->on_error(err);
        }
    };

    Pointer<Context> context = new Context();
    int opts = BEV_OPT_CLOSE_ON_FREE;
    context->bufev = bufferevent_socket_new(base, sockfd, opts);
    bufferevent_setcb(context->bufev, context->do_read, context->do_write,
                      context->do_error, context);
    if (bufferevent_enable(context->bufev, EV_READ) != 0)
        throw std::runtime_error("bufferevent_enable() failed");

    context->transport = make_transport();
    context->transport->send = [context](const void *b, size_t n) {
        if (bufferevent_write(context->bufev, b, n) != 0) {
            throw std::bad_alloc();
        }
    };
    context->transport->close = [context]() {
        auto safe_extra_ref = context;
        if (safe_extra_ref->transport->on_destroy)
            safe_extra_ref->transport->on_destroy();
        safe_extra_ref->transport->on_connect = nullptr;
        safe_extra_ref->transport->on_data = nullptr;
        safe_extra_ref->transport->send = nullptr;
        safe_extra_ref->transport->on_flush = nullptr;
        safe_extra_ref->transport->on_destroy = nullptr;
        safe_extra_ref->transport->on_error = nullptr;
        safe_extra_ref->transport = nullptr;
        bufferevent_free(safe_extra_ref->bufev);
        safe_extra_ref->bufev = nullptr;
    };

    return context->transport;
}
}
