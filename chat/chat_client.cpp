#include <iostream>
#include <boost/asio.hpp>
#include "chat_message.h"
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client {
public:
  chat_client(boost::asio::io_service &io, tcp::resolver::iterator endpoint_iterator)
    : io_service_(io), socket_(io) {
    do_connect(endpoint_iterator);
  }
  
  void write(chat_message &msg) {
    bool empty = write_queue_.empty();
    write_queue_.push_back(msg);
    if (empty) {
      do_write();
    }
  }
  
  void close() {
  }
  
private:
  
  void do_connect(tcp::resolver::iterator endpoint) {
    boost::asio::async_connect(socket_, endpoint,
      [this](boost::system::error_code ec, tcp::resolver::iterator) {
        if (!ec) {
          do_read_header();
        }
      });
  }
  
  void do_read_header() {
    boost::asio::async_read(socket_,
      boost::asio::buffer(msg_.data(), chat_message::header_len),
      [this](boost::system::error_code ec, size_t) {
        if (!ec && msg_.decode_header()) {
            do_read_body();
        } else {
          socket_.close();
        }
      });
  }
  
  void do_read_body() {
    boost::asio::async_read(socket_,
      boost::asio::buffer(msg_.body(), msg_.body_len()),
      [this](boost::system::error_code ec, size_t) {
        if (!ec) {
          std::cout.write(msg_.body(), msg_.body_len());
          std::cout << "\n";
          do_read_header();
        } else {
          socket_.close();
        }
      });
  }
  
  void do_write() {
    chat_message msg = write_queue_.front();
    boost::asio::async_write(socket_,
      boost::asio::buffer(msg.data(), msg.length()),
      [this](boost::system::error_code ec, size_t) {
        if (!ec) {
          write_queue_.pop_front();
          if (!write_queue_.empty())
            do_write();
        } else {
          socket_.close();
        }
      });
  }
  
  boost::asio::io_service &io_service_;
  tcp::socket socket_;
  chat_message msg_;
  chat_message_queue write_queue_;
};

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage " << argv[0] << " host port " << std::endl;
    return 1;
  }
  
  try {
  boost::asio::io_service io_service;
  tcp::resolver resolver(io_service);
  auto iterator = resolver.resolve({argv[1], argv[2]});
  chat_client c(io_service, iterator);
  
  // start a thread to process io service events
  boost::thread t([&io_service]() { io_service.run(); });
  
  // main loop which reads messages from console and sends to
  // server
  char line[chat_message::max_data_len];
  while (std::cin.getline(line, chat_message::max_data_len + 1)) {
    chat_message msg;
    msg.body_len(std::strlen(line));
    msg.encode_header();
    std::memcpy(msg.body(), line, msg.body_len());
    c.write(msg);
  }
  
  c.close();
  t.join();
  } catch (std::exception ex) {
    std::cerr << "Exception occured " << ex.what() << std::endl;
  }
}

