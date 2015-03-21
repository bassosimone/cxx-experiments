/*-
 * This file is part of CXX Experiments.
 *
 * CXX Experiments is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */
#ifndef NET_TEST_HPP
#define NET_TEST_HPP

///
/// \file include/net_test.cpp
/// \brief Base class of network tests
///

#include "pointer.hpp"

#include <iostream>

namespace cxx_experiments {

///
/// \brief Network test that leaks memory.
/// \see doc/net_test.dia for a visual explanation.
///
class LeakingNetTest {
  protected:
    std::function<void()> handle_test;
    std::function<void()> handle_report;

  public:
    void begin(std::function<void()> fn) { handle_test = fn; }

    void trigger_test() { handle_test(); }

    void end(std::function<void()> fn) { handle_report = fn; }

    void trigger_report() { handle_report(); }
};

///
/// \brief Network test that does not leak memory
/// \see doc/net_test.dia for a visual explanation.
///
struct NonLeakingNetTest : public LeakingNetTest {

    void trigger_test() {
        LeakingNetTest::trigger_test();
        handle_test = nullptr;
    }

    void trigger_report() {
        LeakingNetTest::trigger_report();
        handle_report = nullptr;
    }
};

///
/// \brief Check whether there is a leak
/// \return Usage count of test object
///
template <class T> long check_leak() {
#define debug(msg) std::cout << "* " << msg << ": " << test.use_count() << "\n"
    auto test = make<T>();
    debug("after alloc");
    test->begin([=]() {
        debug("inside begin callback");
        test->end([=]() { debug("inside end callback"); });
        debug("after test->end()");
        test->trigger_report();
    });
    debug("after test->begin()");
    test->trigger_test();
    debug("end of main()");
    return test.use_count();
#undef debug
}

}
#endif
