/*-
 * This file is part of CXX Experiments.
 *
 * CXX Experiments is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */

///
/// \file src/pointers_and_containers.cpp
/// \brief Make sure pointers use the pointee value to identify
///        themselves in containers (as one expects it to be)
///

#include "pointer.hpp"
#include "transport.hpp"

#include <iostream>
#include <set>

using namespace cxx_experiments;

class Antani {};

int main() {
    std::set<Pointer<Antani>> set;
    Pointer<Antani> antani = make<Antani>();
    set.insert(antani);
    Pointer<Antani> t2 = antani;
    set.insert(t2);
    Pointer<Antani> t3 = t2;
    set.insert(t3);
    if (set.size() != 1) {
        throw std::runtime_error("unexpected error");
    }
    std::cout << set.size() << "\n";
}
