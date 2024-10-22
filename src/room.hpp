#ifndef LIBCALL_ROOM_HPP
#define LIBCALL_ROOM_HPP

#include "errors/error_t.hpp"
#include "errors/lc_exception.hpp"
#include "session.hpp"
#include "types.hpp"
#include <functional>
#include <memory>
#include <unordered_set>

#include <iostream>

namespace libcall {

class room : public std::enable_shared_from_this<room> {
  id_t id;
  std::unordered_set<std::shared_ptr<session>, shared_ptr_session_hash>
      sessions;

  inline bool is_in(const std::shared_ptr<session> &s) const noexcept {
    if (sessions.find(s) == sessions.end()) {
      return false;
    }

    return true;
  }

  void on_read_handler(const boost::beast::flat_buffer &data) {
    for (auto session : sessions) {
      session->write(data);
    }
  }

public:
  constexpr inline id_t get_id() const noexcept { return id; }

  inline void add_user(id_t id, tcp::socket sock) {
    auto sess = std::make_shared<session>(id, std::move(sock),
                                          std::bind(&room::on_read_handler,
                                                    shared_from_this(),
                                                    std::placeholders::_1));

    if (is_in(sess)) {
      throw errors::lc_exception(
          {errors::error_kind::ALREADY_IN, "User is already in"});
    }

    auto [_, added] = sessions.insert(sess);

    if (!added) {
      throw errors::lc_exception({errors::error_kind::UNKNOWN,
                                  "Ad unknown error occured on adding user!"});
    }

    sess->accept();
  }
};

struct room_hash {
  constexpr inline id_t operator()(const room &room) { return room.get_id(); }
};

} // namespace libcall

#endif