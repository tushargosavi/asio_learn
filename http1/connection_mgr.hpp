#ifndef CONNECTION_MGR_G
#define CONNECTION_MGR_G

#include <set>
#include <boost/noncopyable.hpp>
#include "connection.hpp"

class connection_manager : private boost::noncopyable {
public:
  void start(connection_ptr c);
  void stop(connection_ptr c);
  void stop_all();
private:
  std::set<connection_ptr> connections_;
};

#endif
