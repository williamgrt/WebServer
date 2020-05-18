#ifndef WEBSERVER_SRC_NET_ACCEPTOR_H
#define WEBSERVER_SRC_NET_ACCEPTOR_H

#include "Socket.h"
#include <string>
#include <memory>

namespace web {
class EventLoop;
class Channel;

class Acceptor {
public:
  Acceptor(const std::string &hostname, unsigned short port);
  ~Acceptor();

private:
  Socket socket_;
  Ip4Addr addr_;
  std::unique_ptr<Channel> socketChannel_;

};

}

#endif //WEBSERVER_SRC_NET_ACCEPTOR_H
