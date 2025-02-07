#include <ctime>
#include <coroutine>
#include <iostream>
#include <string>
#include <memory>

#include <asio.hpp>

std::string make_daytime_string()
{
  std::time_t now = std::time(0);
  return std::ctime(&now);
}

class tcp_connection : std::enable_shared_from_this<tcp_connection> {
public:
  using pointer = std::shared_ptr<tcp_connection>;

  static pointer create(asio::io_context& io_context) {
    return std::shared_ptr<tcp_connection>(new tcp_connection(io_context));
  }

  asio::ip::tcp::socket& socket() {
    return socket_;
  }

  void start() {
    message_ = make_daytime_string();

    asio::async_write(socket_, asio::buffer(message_),
        std::bind(&tcp_connection::handle_write, shared_from_this(),
          std::placeholders::_1,
          std::placeholders::_2));
  }

private:
  tcp_connection(asio::io_context& io_context)
    : socket_(io_context)
  {
  }

  void handle_write(const asio::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  asio::ip::tcp::socket socket_;
  std::string message_;
};

class tcp_server {
public:
  tcp_server(asio::io_context& io_context)
      : io_context_(io_context)
      , acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13)) {
    start_accept();
  }

private:
  void start_accept() {
    tcp_connection::pointer new_connection =
      tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
        std::bind(&tcp_server::handle_accept, this, new_connection,
          std::placeholders::_1));
  }

  void handle_accept(tcp_connection::pointer new_connection,
                     const asio::error_code& error) {
    if (!error) {
      new_connection->start();
    }

    start_accept();
  }

  asio::io_context& io_context_;
  asio::ip::tcp::acceptor acceptor_;
};

int main()
{
  try
  {
    asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }


  return 0;
}