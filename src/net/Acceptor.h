#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "EventLoop.h"
#include "Socket.h"

namespace gnet {

class Acceptor {
private:
  int sockfd_;
  Ip4Addr addr_;
  EventLoop *ev_;

public:
  Acceptor(EventLoop *ev);
  ~Acceptor();

  // 创建连接
  void Init();
  void Bind(const std::string &host, unsigned short port);
  void Listen(int maxWaitNum);

  EventLoop *GetEv() { return ev_; }
  int GetSockFd() { return sockfd_; }
  Ip4Addr &GetAddr() { return addr_; }
};

} // namespace gnet

#endif // _ACCEPTOR_H