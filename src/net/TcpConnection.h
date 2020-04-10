#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "../base/NonCopyable.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

namespace gnet {
class TcpConnection : public NonCopyable {
private:
  int connfd_;
  EventLoop *ev_;
  Ip4Addr local_, peer_;

public:
  TcpConnection();
  ~TcpConnection();

  void Init(int connfd, const Ip4Addr &local);
};

TcpConnection::TcpConnection(/* args */) {}

TcpConnection::~TcpConnection() {}

} // namespace gnet

#endif // _TCPCONNECTION_H