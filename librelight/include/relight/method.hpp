//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

#include <functional>

namespace relight {

template <typename T> class Method; // unspecified

template <typename R, typename... Args> class Method<R(Args...)> {

    std::function<R(Args...)> func;

  public:
    Method() {}

    template <class Callable> Method(Callable c) : func(c) {}

    template <class Callable> Method &operator=(Callable c) {
        func = c;
        return *this;
    }

    R operator()(Args ... args) {
        auto copy = func;
        return copy(args...);
    }

    operator bool() {
        return bool(func);
    }
};

}
