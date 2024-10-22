#ifndef LIBCALL_ERRORS_ERROR_T_HPP
#define LIBCALL_ERRORS_ERROR_T_HPP

#include <string>

namespace libcall::errors {

enum struct error_kind { OK, NOT_A_WEBSOCKET, ALREADY_IN, UNKNOWN };

struct error_t {
  error_kind kind;
  std::string message;

  inline operator bool() { return kind == error_kind::OK; }
};

} // namespace libcall::errors

#endif