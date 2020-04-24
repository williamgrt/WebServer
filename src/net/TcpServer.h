#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Utils.h"

namespace web {

class TcpServer {
public:
  using ReadHandler = std::function<void()>;

private:
  std::unique_ptr<Acceptor> acceptor_;
  EventLoop *ev_;
  types::ChannelPtr liveChannel_;
  int state_;

public:
  TcpServer(EventLoop *ev);
  ~TcpServer();

  void OnRead();

  types::TcpServerPtr Start(const std::string &host, unsigned short port);
  void HandleAccept();
};

} // namespace web

#endif // _TCPSERVER_H