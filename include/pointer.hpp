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
    T *operator->() const {
        if (std::shared_ptr<T>::get() == nullptr) {
            throw std::runtime_error("Null pointer deference");
        }
        return std::shared_ptr<T>::operator->();
    }

    typename std::add_lvalue_reference<T>::type operator*() const {
        if (std::shared_ptr<T>::get() == nullptr) {
            throw std::runtime_error("Null pointer deference");
        }
        return std::shared_ptr<T>::operator*();
    }
};

template <typename T, typename... A> Pointer<T> make(A &&... args) {
    return Pointer<T>(new T(std::forward<A>(args)...));
}

}
#endif
