#ifndef LIBCALL_USER_HPP
#define LIBCALL_USER_HPP

#include <boost/beast/core/bind_handler.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <boost/core/ignore_unused.hpp>
#include <memory>

namespace libcall {

using tcp = boost::beast::net::ip::tcp;

// class user {
//   uint64_t id;
//   tcp::socket sock;

// public:
//   inline user(uint64_t id, tcp::socket socket)
//       : id(id), sock(std::move(socket)) {}

//   user(const user &) = delete;

//   inline user(user &&other) : id(other.id), sock(std::move(other.sock)) {}

//   constexpr inline uint64_t get_id() const noexcept { return id; }

//   constexpr inline tcp::socket &get_socket() noexcept { return sock; }
// };

// struct user_hasher {
//   constexpr inline uint64_t operator()(const user &user) const noexcept {
//     return user.get_id();
//   }
// };

class session : public std::enable_shared_from_this<session> {
  using on_read_handler =
      std::function<void(const boost::beast::flat_buffer &buffer)>;

  id_t id;
  boost::beast::websocket::stream<boost::beast::tcp_stream> sock;
  boost::beast::flat_buffer buffer;
  on_read_handler handler;

  inline void on_accept(boost::beast::error_code ec) {
    if (!ec) {
      do_read();
    }
  }

  inline void do_read() {
    sock.async_read(buffer, boost::beast::bind_front_handler(
                                &session::on_read, this->shared_from_this()));
  }

  inline void on_read(boost::beast::error_code ec, size_t bytes_transferred) {
    handler(buffer);
    do_read();
  }

  inline void on_write(boost::beast::error_code ec, size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    buffer.consume(buffer.size());
  }

public:
  inline session(id_t id, tcp::socket sock, on_read_handler handler)
      : id(id), sock(std::move(sock)), buffer(), handler(handler) {}

  session(const session &s) = delete;

  session &operator=(const session &s) = delete;

  inline session(session &&other) = delete;

  inline session &operator=(session &&other) = delete;

  constexpr inline id_t get_id() const noexcept { return id; }

  inline void accept() noexcept {
    sock.async_accept(boost::beast::bind_front_handler(
        &session::on_accept, this->shared_from_this()));
  }

  inline void write(const boost::beast::flat_buffer &data) {
    sock.async_write(buffer.data(),
                     boost::beast::bind_front_handler(
                         &session::on_write, this->shared_from_this()));
  }
};

struct shared_ptr_session_hash {
  constexpr inline uint64_t
  operator()(const std::shared_ptr<session> &s) const noexcept {
    return s->get_id();
  }
};

constexpr inline bool operator==(const std::shared_ptr<session> &f,
                                 const std::shared_ptr<session> &s) {
  return f->get_id() == s->get_id();
}

} // namespace libcall

#endif