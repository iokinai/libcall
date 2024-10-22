#ifndef LIBCALL_ERRORS_LC_EXCEPTION_HPP
#define LIBCALL_ERRORS_LC_EXCEPTION_HPP

#include "error_t.hpp"
#include <exception>
#include <magic_enum/magic_enum.hpp>
#include <sstream>

namespace libcall::errors {

class lc_exception : public std::exception {
  error_t err;
  std::string message;

  inline void setup_message() noexcept {
    std::stringstream ss;
    ss << "An error occured!\nKind: " << magic_enum::enum_name(err.kind)
       << " with message: " << err.message;

    message = ss.str();
  }

public:
  inline lc_exception(error_t error) : err(error) { setup_message(); }

  virtual inline const char *what() const noexcept override {
    return message.c_str();
  }

  constexpr inline const error_t &get_error() const noexcept { return err; }
};

} // namespace libcall::errors

#endif