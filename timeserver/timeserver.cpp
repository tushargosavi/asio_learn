#include <ctime>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <iostream>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
  using namespace std;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
public:
  typedef boost::shared_ptr<tcp_connection> pointer;
  
  static pointer create(boost::asio::io_service &io) {
    return pointer(new tcp_connection(io));
  }
  
  tcp::socket& socket() {
    return socket_;
  }
  
  void start() {
    message_ = make_daytime_string();
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
                             boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
                             
  }
  
private:
  
  tcp_connection(boost::asio::io_service &io):
  socket_(io) {
  }
  
  void handle_write(const boost::system::error_code &ec, size_t bytes) {
  }
  
  tcp::socket socket_;
  std::string message_;
};

class tcp_server {
public:
  tcp_server(boost::asio::io_service &io):
  acceptor(io, tcp::endpoint(tcp::v4(), 13)) {
    start_accept();
  }
  
private:
  
  void start_accept() {
    tcp_connection::pointer new_connection = tcp_connection::create(acceptor.get_io_service());
    acceptor.async_accept(new_connection->socket(), 
                    boost::bind(&tcp_server::handle_accept, this, new_connection,
                                boost::asio::placeholders::error));
  }
  
  void handle_accept(tcp_connection::pointer connection, const boost::system::error_code& ec) {
    if (!ec) {
      connection->start();
    }
    
    start_accept();
  }
  
  tcp::acceptor acceptor;
};

int main(int argc, char **argv) {
  try {
    boost::asio::io_service io;
    tcp_server server(io);
    io.run();
  } catch (std::exception &e) {
    std::cerr << "Exception " << e.what() << std::endl;
  }
  return 0;
}

