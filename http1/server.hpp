
#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "connection_mgr.hpp"

using boost::asio::ip::tcp;

class server : public std::enable_shared_from_this<server> {
  
public:
  
  server(int port);
    
  void run();
  
private:
  
  void do_accept();
  
  int port_;
  std::string root_dir_;
  boost::asio::io_service io_service_;
  tcp::endpoint endpoint_;
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  connection_manager cmgr_;
};

#endif
