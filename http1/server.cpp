#include <iostream>
#include <boost/asio.hpp>

#include "server.hpp"
#include "connection.hpp"

using boost::asio::ip::tcp;

server::server(int port):
  port_(port),
  endpoint_(tcp::v4(), port),
  io_service_(),
  socket_(io_service_),
  acceptor_(io_service_, endpoint_)
{
}

void server::run() {
  std::cout << "starting server on port " << port_ << std::endl;
  do_accept();
  io_service_.run();
}
  
void server::do_accept() {
  acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec) {
			  if (!ec) {
			    std::cout << "new connection " << std::endl;
			    cmgr_.start(std::make_shared<connection>(std::move(socket_)));
			  }
			  do_accept();
			});
}
