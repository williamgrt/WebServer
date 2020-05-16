#ifndef WEBSERVER_SRC_NET_ACCEPTOR_H
#define WEBSERVER_SRC_NET_ACCEPTOR_H

#include "Socket.h"
#include <string>

namespace web {

class Acceptor {
public:

  Acceptor(const std::string &hostname, unsigned short port);
  ~Acceptor();

private:
  Socket socket_;
  Ip4Addr addr_;

};

}

#endif //WEBSERVER_SRC_NET_ACCEPTOR_H
