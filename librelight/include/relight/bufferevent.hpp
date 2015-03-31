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

class BufferEventSocket {
    std::shared_ptr<bufferevent> bufev;

  public:
    BufferEventSocket() {}

    void init(EventBase base, evutil_socket_t s) {
        bufferevent *ptr = bufferevent_socket_new(base, s,
                               BEV_OPT_CLOSE_ON_FREE);
        if (ptr == nullptr)
            throw std::bad_alloc();
        bufev = std::shared_ptr<bufferevent>{
            ptr, bufferevent_free
        };
    }

    operator bufferevent *() {
        bufferevent *ptr = bufev.get();
        if (ptr == nullptr)
            throw std::runtime_error("null pointer");
        return ptr;
    }
};

Pointer<Transport> bufferevent_attach(EventBase base, evutil_socket_t sockfd) {

    struct Context {
        Pointer<Transport> transport;
        BufferEventSocket bufev;

        static void do_read(bufferevent *, void *opaque) {
            Pointer<Transport> transport;
            BufferEventSocket bufev;

            // Make a copy of objects we use to keep them alive
            // until the end of the scope: Context may be kept
            // alive only by method closures that could be reset
            // if close() is called during on_data()
            {
                Context *ctx = static_cast<Context *>(opaque);
                transport = ctx->transport;
                bufev = ctx->bufev;
            }

            evbuffer *buffer = bufferevent_get_input(bufev);

            ssize_t ctrl = evbuffer_peek(buffer, -1, NULL, NULL, 0);
            if (ctrl <= 0)
                throw std::runtime_error("evbuffer_peek() failed");
            size_t size = static_cast<size_t>(ctrl);
            EvBufferIovec iovec{size};
            if (evbuffer_peek(buffer, -1, NULL, iovec, size) != ctrl)
                throw std::runtime_error("evbuffer_peek() failed");

            for (size_t i = 0; i < size; i++)
                transport->on_data(iovec[i]->iov_base, iovec[i]->iov_len);
                if (!transport->on_data)
                    break;  // Close was called and on_data() was reset
        }

        static void do_write(bufferevent *, void *opaque) {
            Context *ctx = static_cast<Context *>(opaque);
            ctx->transport->on_flush();
        }

        static void do_error(bufferevent *, short what, void *opaque) {
            Context *ctx = static_cast<Context *>(opaque);
            ctx->transport->on_error(new Error(what));
        }
    };

    Pointer<Context> context = new Context();
    context->bufev->init(base, sockfd);
    bufferevent_setcb(context->bufev, context->do_read, context->do_write,
                      context->do_error, static_cast<Context *>(context));
    if (bufferevent_enable(context->bufev, EV_READ) != 0)
        throw std::runtime_error("bufferevent_enable() failed");

    context->transport = make_transport();
    context->transport->send = [context](const void *b, size_t n) {
        if (bufferevent_write(context->bufev, b, n) != 0)
            throw std::bad_alloc();
    };
    context->transport->close = [context]() {
        if (context->transport->on_destroy)
            context->transport->on_destroy();

        // Clear the callbacks to remove circular references
        context->transport->on_connect = nullptr;
        context->transport->on_data = nullptr;
        context->transport->send = nullptr;
        context->transport->on_flush = nullptr;
        context->transport->on_destroy = nullptr;
        context->transport->on_error = nullptr;
    };

    return context->transport;
}
}
