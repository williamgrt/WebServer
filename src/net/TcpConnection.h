#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "../base/noncopyable.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

namespace web {
class TcpConnection : public noncopyable {
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

} // namespace web

#endif // _TCPCONNECTION_H