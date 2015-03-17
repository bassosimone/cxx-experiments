/*-
 * This file is part of CXX Experiments.
 *
 * CXX Experiments is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */
#ifndef TRANSPORT_HPP
#define TRANSPORT_HPP

///
/// \file include/transport.hpp
/// \brief Mocked transport
///

#include <functional>
#include <string>

#include "constraints.hpp"

namespace cxx_experiments {

class Transport : public NonCopyable, public NonMovable {

  std::function<void()> connect_handler;
  std::function<void(std::string)> data_handler;
  std::function<void()> flush_handler;
  std::function<void(int)> error_handler;

  int filedesc;

public:
  Transport(int fd = -1) : filedesc(fd) {}

  void on_connect(std::function<void()> fn) { connect_handler = fn; }
  void on_data(std::function<void(std::string)> fn) { data_handler = fn; }
  void on_flush(std::function<void()> fn) { flush_handler = fn; }
  void on_error(std::function<void(int)> fn) { error_handler = fn; }

  void emit_connect() { connect_handler(); }
  void emit_data(std::string d) { data_handler(d); }
  void emit_flush() { flush_handler(); }
  void emit_error(int e) { error_handler(e); }

  int get_filedesc() { return filedesc; }

  void close() {
    connect_handler = nullptr;
    data_handler = nullptr;
    flush_handler = nullptr;
    error_handler = nullptr;
    filedesc = -1;
  }
};

}
#endif
