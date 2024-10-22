#ifndef LIBCALL_CALL_HPP
#define LIBCALL_CALL_HPP

#include "room.hpp"
#include <unordered_set>

namespace libcall {

class call final {
  std::unordered_set<room> rooms;

public:
};

} // namespace libcall

#endif