/*-
 * This file is part of CXX Experiments.
 *
 * CXX Experiments is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */

///
/// \file src/closure_slots.cpp
/// \brief Demonstrates memory error on closure.
///

#include "transport.hpp"

using namespace cxx_experiments;

int main() { closure_example<TransportWrong>(); }
