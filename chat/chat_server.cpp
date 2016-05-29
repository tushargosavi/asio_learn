#include <iostream>
#include <boost/asio.hpp>
#include <deque>
#include <set>
#include "chat_message.h"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_participant {
public:
  virtual ~chat_participant() {}
  virtual void deliver(const chat_message &msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_room {
public:
  void join(chat_participant_ptr participant) {
    participants.insert(participant);
    std::cout << "joining clinet total " << participants.size() << std::endl;
    for (chat_message msg : recent_messages) {
      participant->deliver(msg);
    }
  }
  
  void leave(chat_participant_ptr participant) {
    participants.erase(participant);
  }
  
  void deliver(chat_message &msg) {
    while (recent_messages.size() > max_recent_messages)
      recent_messages.pop_front();
    recent_messages.push_back(msg);
    
    for (auto participant : participants) {
      participant->deliver(msg);
    }
    
  }
  
private:
  chat_message_queue recent_messages;
  enum { max_recent_messages = 100 };
  std::set<chat_participant_ptr> participants;
  
};

class chat_session : 
  public chat_participant,
  public std::enable_shared_from_this<chat_session> {
    
public:
  chat_session(tcp::socket socket, chat_room& room):
    socket_(std::move(socket)), room_(room) {
      
  }
  
  void start() {
    room_.join(shared_from_this());
    do_read_header();
  }
  
  void deliver(const chat_message &msg) {
    bool write_in_progress = write_msgs_.size() > 0;
    write_msgs_.push_back(msg);
    if (!write_in_progress) {
      do_write();
    }
  }
  
private:        
  
  void do_read_header() {
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
      boost::asio::buffer(read_msg_.data(), chat_message::header_len),
      [this, self](boost::system::error_code ec, std::size_t) {
        if (!ec && read_msg_.decode_header()) {
          do_read_body();
        } else {
          room_.leave(shared_from_this());
          socket_.close();
        }
      });
  }
  
  void do_read_body() {
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
      boost::asio::buffer(read_msg_.body(), read_msg_.body_len()),
      [this, self](boost::system::error_code ec, size_t) {
        if (!ec) {
          room_.deliver(read_msg_);
          do_read_header();
        } else {
          room_.leave(shared_from_this());
          socket_.close();
        }
      });
  }
  
  void do_write() {
    auto self = shared_from_this();
    chat_message msg = write_msgs_.front();
    boost::asio::async_write(socket_,
      boost::asio::buffer(msg.data(), msg.length()),
      [this, self](boost::system::error_code ec, size_t) {
        if (!ec) {
          write_msgs_.pop_front();
          if (!write_msgs_.empty())
            do_write();
        } else {
          room_.leave(shared_from_this());
          socket_.close();
        }
      });
  }
  
  tcp::socket socket_;
  chat_room &room_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

class chat_server {
public:
  chat_server(boost::asio::io_service &io, const tcp::endpoint &endpoint) :
    acceptor_(io, endpoint),
    socket_(io) {
    do_accept();
  }
  
private:
  void do_accept() {
    acceptor_.async_accept(socket_,
                          [this](boost::system::error_code ec) {
                            if (!ec) {
                              std::make_shared<chat_session>(std::move(socket_), room_)->start();
                            }
                            
                            do_accept();
                          });
  }
  
private:
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  chat_room room_;
};

int main(int argc, char **argv) {
  try {
    if (argc < 2) {
      std::cerr << "Usage " << argv[0] << " port " << std::endl;
      return 1;
    }
    
    boost::asio::io_service io;
    tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
    chat_server server(io, endpoint);
    io.run();
  } catch (std::exception e) {
    std::cerr << "exception occured " << e.what() << std::endl;
  }
  return 0;
};
