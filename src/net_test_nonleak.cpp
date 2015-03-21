/*-
 * This file is part of CXX Experiments.
 *
 * CXX Experiments is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */

///
/// \file src/net_test_leak.cpp
/// \brief Show that the nonleaking net test does not leak
///

#include "net_test.hpp"

using namespace cxx_experiments;

int main() { (void)check_leak<NonLeakingNetTest>(); }
