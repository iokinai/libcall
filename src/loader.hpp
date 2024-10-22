#ifndef LIBCALL_LOADER_HPP
#define LIBCALL_LOADER_HPP

#include "aliases.h"
#include <boost/beast.hpp>

namespace libcall {

using tcp = boost::beast::net::ip::tcp;

void throw_if_not_websocket(const tcp::socket &sock) {
  //
}

void transfer_socket(id_t user_id, id_t room_id, tcp::socket sock) {}

} // namespace libcall

#endif