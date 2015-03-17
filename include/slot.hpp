/*-
 * This file is part of CXX Experiments.
 *
 * CXX Experiments is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */
#ifndef SLOT_HPP
#define SLOT_HPP

///
/// \file include/slot.hpp
/// \brief A slot to extend a class
///

#include <functional>

namespace cxx_experiments {

template <typename T> class Slot {  // Inspired by std::function<> API
  std::function<T> function;

public:
  Slot() {}
  Slot(std::function<T> f) : function(f) {}

  // No need to implement a destructor

  void operator=(std::function<T> f) { function = f; }
  void operator=(std::nullptr_t f) { function = f; }

  // No need to implement swap() and assign()

  operator bool() { return bool(function); }

  template <typename... Args> void operator()(Args&&... args) {
    //
    // The following code guarantees that the closure bound to function
    // is not destroyed before the end of this scope.
    //
    // This is important because, if the code called by function changes
    // the value of function, the old closure is destroyed.
    //
    // If the old closure was used to guarantee that the object used by
    // the code called by function was alive, we have use after free.
    //
    auto backup = function;
    backup(std::forward<Args>(args)...);
  }
};

}
#endif
