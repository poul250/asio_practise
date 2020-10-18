#include <iostream>
#include <string>
#include <functional>
#include <memory>

#include <asio/ts/internet.hpp>
#include <asio.hpp>

namespace {

using tcp = asio::ip::tcp;

template<typename ConnectionHandler>
struct connection {
  connection(asio::io_context& io_context, size_t buffer_size = 1024) 
      : socket_(io_context)
      , buffer_(buffer_size) {}
  connection(const connection&) = delete;
  connection(connection&&) = default;

  void data_recieved(asio::error_code ec, std::shared_ptr<ConnectionHandler> conn) {
    conn->on_data_recieved(std::string(buffer_.begin(), buffer_.end()));
    socket_.async_read_some(asio::buffer(buffer_),
      std::bind(&connection::data_recieved, this, std::placeholders::_1, std::move(conn)));
  }

  void start(std::shared_ptr<ConnectionHandler> handler) {
    socket_.async_read_some(asio::buffer(buffer_), 
        std::bind(&connection::data_recieved, this, std::placeholders::_1, std::move(handler)));
  }

  void write_data(std::string data) {
    socket_.async_write_some(asio::buffer(std::move(data)), 
      std::bind(&connection::write_handler, this, std::placeholders::_1, std::placeholders::_2));
  }

  tcp::socket& socket() {
    return socket_;
  }
private:
  void write_handler(asio::error_code, size_t size) {
  }
  tcp::socket socket_;
  std::vector<char> buffer_;
};

class echo_connection : public connection<echo_connection> {
public:
  using connection<echo_connection>::connection;

  void on_data_recieved(std::string data) {
    write_data(std::move(data));
  }
};

class echo_server {
public:
  echo_server(asio::io_context& io_context, unsigned short port = 80u)
      : io_context_(io_context)
      , acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
  }

private:
  void start_accept() {
    auto conn = std::make_shared<echo_connection>(io_context_);
    acceptor_.async_accept(conn->socket(), 
        std::bind(&echo_server::handle_accept, this, std::placeholders::_1, conn));
  }

  void handle_accept(asio::error_code ec, std::shared_ptr<echo_connection> conn) {
    if (!ec) {
      conn->start(std::move(conn));
    }
    start_accept();
  }

  asio::io_context& io_context_;
  asio::ip::tcp::acceptor acceptor_;
};

}  // namespace

int main() {
  asio::io_context io_context;
  echo_server server(io_context);
  io_context.run();

  return 0;
}
