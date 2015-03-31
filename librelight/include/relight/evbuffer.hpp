//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

#include <memory>
#include <stdexcept>

#include <event2/buffer.h>

namespace relight {

class EvBufferIovec {

    std::shared_ptr<iovec> iov;
    size_t size_;

  public:
    EvBufferIovec(size_t s) {
        if (s == 0)
            throw std::runtime_error("invalid size");
        iov.reset(new iovec[s]);
        size_ = s;
    }

    iovec *operator[](size_t index) {
        if (index >= size_)
            throw std::runtime_error("bad index");
        return iov.get() + index;
    }

    operator iovec *() { return iov.get(); }

    size_t size() { return size_; }
};

class EvBuffer {
    std::shared_ptr<evbuffer> evbuf{
        evbuffer_new(),
        EvBuffer::free
    };

    static void free(evbuffer *b) {
        if (b != nullptr)
            evbuffer_free(b);
    }

  public:
    operator evbuffer *() {
        auto ptr = evbuf.get();
        if (ptr == nullptr)
            throw std::runtime_error("null pointer");
        return ptr;
    }
};
}
