#include <ctime>
#include <boost/asio.hpp>
#include <string>
#include <iostream>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
  using namespace std;
  time_t now = time(0);
  return ctime(&now);
}

int main(int argc, char **argv) {
  try {
    boost::asio::io_service io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));
    for (;;) {
      tcp::socket socket(io);
      acceptor.accept(socket);
      std::string str = make_daytime_string();
      boost::system::error_code error;
      boost::asio::write(socket, boost::asio::buffer(str), error);
    }
  } catch (std::exception &e) {
    std::cerr << "Exception " << e.what() << std::endl;
  }
  return 0;
}

