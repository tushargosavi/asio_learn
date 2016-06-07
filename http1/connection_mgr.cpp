#include "connection_mgr.hpp"

void connection_manager::start(connection_ptr c) {
  connections_.insert(c);
  c->start();
}

void connection_manager::stop(connection_ptr c) {
  connections_.erase(c);
}

void connection_manager::stop_all()
{
  for (connection_ptr p : connections_) {
    p->stop();
  }
  connections_.clear();
}
