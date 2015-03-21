/*-
 * This file is part of CXX Experiments.
 *
 * CXX Experiments is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */
#ifndef TRANSPORT_HPP
#define TRANSPORT_HPP

///
/// \file include/transport.hpp
/// \brief Model of libight's transport
///

#include "constraints.hpp"
#include "pointer.hpp"
#include "slot.hpp"

#include <iostream>
#include <string>

namespace cxx_experiments {

template <template <typename T> class F>
class Transport : public NonCopyable, public NonMovable {

  private:
    int filedesc = -1;

  protected:
    F<void()> connect_handler;
    F<void(std::string)> data_handler;
    F<void()> flush_handler;
    F<void(int)> error_handler;

  public:
    Transport(int fd = -1) : filedesc(fd) {}

    void on_connect(std::function<void()> fn) { connect_handler = fn; }
    void on_data(std::function<void(std::string)> fn) { data_handler = fn; }
    void on_flush(std::function<void()> fn) { flush_handler = fn; }
    void on_error(std::function<void(int)> fn) { error_handler = fn; }

    virtual void emit_connect() { connect_handler(); }
    virtual void emit_data(std::string d) { data_handler(d); }
    virtual void emit_flush() { flush_handler(); }
    virtual void emit_error(int e) { error_handler(e); }

    int get_filedesc() { return filedesc; }

    void close() {
        connect_handler = nullptr;
        data_handler = nullptr;
        flush_handler = nullptr;
        error_handler = nullptr;
        filedesc = -1;
    }

    virtual ~Transport(){};
};

struct TransportWithSlots : public Transport<Slot> {
    using Transport::Transport;
};

struct TransportWrong : public Transport<std::function> {
    using Transport::Transport;
};

struct TransportManual : public Transport<std::function> {
    using Transport::Transport;

    virtual void emit_connect() override {
        auto fn = connect_handler;
        fn();
    }

    virtual void emit_data(std::string d) override {
        auto fn = data_handler;
        fn(d);
    }

    virtual void emit_flush() override {
        auto fn = flush_handler;
        fn();
    }

    virtual void emit_error(int e) override {
        auto fn = error_handler;
        fn(e);
    }
};

template <class T> struct VerboseTransport : public T {
    VerboseTransport(int fd = -1) : T(fd) {
        std::cout << "Construct " << this << "\n";
    }

    virtual ~VerboseTransport() { std::cout << "Destruct " << this << "\n"; }

    virtual void emit_connect() override {
        std::cout << "Connect " << this << "\n";
        T::emit_connect();
    }
};

template <class T> void closure_example() {
    auto pointer = []() {
        auto transport = make<VerboseTransport<T>>(12345);
        transport->on_connect([transport]() { transport->close(); });
        //
        // Here we only return the raw pointer and the reference count of
        // transport is decremented because we leave the scope.
        //
        // However, transport is still alive because it is saved in the
        // closure of the on_connect() callback.
        //
        // We use the raw pointer as dirty trick to use the transport from
        // the main function without changing its refcnt.
        //
        return transport.get();
    }();

    //
    // Now we emit connect() which triggers the above callback which
    // closes the transport which in turn means that all the callbacks
    // are reset. That is, the closure of the callbacks is erased,
    // hence the last reference to transport is gone.
    //
    pointer->emit_connect();
}

}
#endif
