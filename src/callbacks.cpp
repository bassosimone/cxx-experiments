/*-
 * This file is part of CXX Experiments.
 *
 * CXX Experiments is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */

///
/// \file src/callbacks.cpp
/// \brief Example with callbacks
///

#include "pointer.hpp"
#include "transport.hpp"

#include <iostream>

using namespace cxx_experiments;

class MyTransport : public Transport {
public:
  MyTransport(int fd = -1) : Transport(fd) {
    std::cout << "Construct " << this << "\n";
  }

  ~MyTransport() { std::cout << "Destruct " << this << "\n"; }

  void emit_connect() {
    std::cout << "Connect " << this << "\n";
    Transport::emit_connect();
  }
};

static MyTransport *create_transport() {
  auto transport = make<MyTransport>(12345);
  transport->on_connect([transport]() {
    transport->close();
  });

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
}

int main() {
  auto pointer = create_transport();
  std::cout << "Returned\n";

  //
  // Now we emit connect() which triggers the above callback which
  // closes the transport which in turn means that all the callbacks
  // are reset. That is, the closure of the callbacks is erased,
  // hence the last reference to transport is gone.
  //
  pointer->emit_connect();
}
