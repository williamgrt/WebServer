#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include "Acceptor.h"

#include <memory>

namespace gnet {
class TcpServer {
private:
  std::unique_ptr<Acceptor> acceptor_;
  EventLoop *ev_;
  int running;

public:
  TcpServer(EventLoop *ev);
  ~TcpServer();
};

} // namespace gnet

#endif // _TCPSERVER_H