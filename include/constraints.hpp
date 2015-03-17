/*-
 * This file is part of CXX Experiments.
 *
 * CXX Experiments is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */
#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

///
/// \file include/constraints.hpp
/// \brief Constraints for objects
///

namespace cxx_experiments {

class NonCopyable {
public:
  NonCopyable() {}
  NonCopyable(NonCopyable &) = delete;
  NonCopyable &operator=(NonCopyable &) = delete;
};

class NonMovable {
public:
  NonMovable() {}
  NonMovable(NonMovable &&) = delete;
  NonMovable &operator=(NonMovable &&) = delete;
};

}
#endif
