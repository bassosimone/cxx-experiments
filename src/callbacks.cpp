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

using namespace cxx_experiments;

static void create_transport() {
  Pointer<Transport> transport(new Transport());
  transport->on_connect([transport]() {
    ; /* nothing */
  });
}

int main() { create_transport(); }
