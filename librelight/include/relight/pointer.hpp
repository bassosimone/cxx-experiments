//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

#include <memory>
#include <stdexcept>

namespace relight {

template <typename T> class Pointer {

    std::shared_ptr<T> pointer;

  public:
    Pointer() {}
    template <class Deleter> Pointer(T *p, Deleter d) : pointer(p, d) {}
    Pointer(T *p) : pointer(p) {}

    Pointer &operator=(T *p) {
        pointer.reset(p);
        return *this;
    }

    operator bool() const { return bool(pointer); }

    operator T *() const {
        if (!pointer) {
            throw std::runtime_error("null pointer");
        }
        return pointer.get();
    }

    T *operator->() const {
        if (!pointer) {
            throw std::runtime_error("null pointer");
        }
        return pointer.get();
    }

    T *get() const {
        if (!pointer) {
            throw std::runtime_error("null pointer");
        }
        return pointer.get();
    }
};
}
