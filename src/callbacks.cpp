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
};

static void create_transport() {
  auto transport = make<MyTransport>(12345);
  transport->on_connect([transport]() {
    ; /* nothing */
  });
}

int main() { create_transport(); }
