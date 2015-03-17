/*-
 * This file is part of CXX Experiments.
 *
 * CXX Experiments is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */
#ifndef POINTER_HPP
#define POINTER_HPP

///
/// \file include/pointer.hpp
/// \brief Smart pointer
///

#include <memory>
#include <stdexcept>

namespace cxx_experiments {

template <typename T> class Pointer : public std::shared_ptr<T> {

  using std::shared_ptr<T>::shared_ptr;

public:
  T *operator->() {
    if (std::shared_ptr<T>::get() == nullptr) {
      throw std::runtime_error("Null pointer deference");
    }
    return std::shared_ptr<T>::operator->();
  }

  typename std::add_lvalue_reference<T>::type operator*() {
    if (std::shared_ptr<T>::get() == nullptr) {
      throw std::runtime_error("Null pointer deference");
    }
    return std::shared_ptr<T>::operator*();
  }
};

}
#endif
