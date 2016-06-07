#ifndef CONNECTION_H
#define CONNECTION_H

#include <vector>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

class connection {
public:
  typedef boost::asio::buffers_iterator<
    boost::asio::streambuf::const_buffers_type> iterator;
    
  connection(tcp::socket socket):socket_(std::move(socket)) {
  }
  
  void start() {
    do_read_request();
  }
  
  void stop() {
    socket_.close();
  }
  
private:
  
  void do_read_request() {
    std::cout << "Waiting for read message " << std::endl;
   socket_.async_read_some(boost::asio::buffer(line),
			    [this](boost::system::error_code ec, size_t size) {
			      if (!ec) {
				std::cout << "data received size " << size << std::endl;
				socket_.close();
			      } else {
				socket_.close();
			      }
			    });
  }
    
  tcp::socket socket_;
  std::string request;
  boost::array<char, 128> line;
};

typedef std::shared_ptr<connection> connection_ptr;

#endif