#include <iostream>
#include <asio.hpp>

void print(const asio::error_code& e)
{
  if (e) {
    std::cout << "Error: " << e.message() << std::endl;
  }
  std::cout << "Hello, world!" << std::endl;
}

void print2(const asio::error_code& e, asio::steady_timer* timer, int count) {
  std::cout << count << std::endl;

  if (count < 5) {
    timer->expires_at(timer->expiry() + asio::chrono::seconds(1));
    timer->async_wait(std::bind(print2, std::placeholders::_1, timer, count+1));
  }
}

class printer {
public:
  printer(asio::io_context& io) : timer{io, asio::chrono::seconds{1}}, count{0} {
    timer.async_wait(std::bind(&printer::print, this));
  }

  void print() {
    if (count < 5)
    {
      std::cout << count << std::endl;
      ++count;

      timer.expires_at(timer.expiry() + asio::chrono::seconds(1));
      timer.async_wait(std::bind(&printer::print, this));
    }
  }

  ~printer() {
    std::cout << "done";
  }

private:
  asio::steady_timer timer;
  int count;
};

int main()
{
  asio::io_context io;

  // synchronous timer
  asio::steady_timer t{io, asio::chrono::seconds{5}};
  t.wait();
  std::cout << "Hello, world!" << std::endl;

  // asynchronous variant
  asio::steady_timer at{io, asio::chrono::seconds{5}};
  at.async_wait(&print);
  io.run();
  io.restart();

  // bind expample
  asio::steady_timer bind_timer{io, asio::chrono::seconds{1}};
  bind_timer.async_wait(std::bind(print2, std::placeholders::_1, &bind_timer, 0));
  io.run();
  io.restart();

  // class object
  printer pr(io);
  io.run();

  return 0;
}