#include <iostream>
#include <boost/asio.hpp>

#include "server.hpp"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage " << argv[0] << " port " << std::endl;
    return 1;
  }

  int port = std::atoi(argv[1]);
  server server(port);
  server.run();
  
  std::cout << "Starting http server on port " << argv[1] << std::endl;
  return 0;
}
