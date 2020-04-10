#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "../base/NonCopyable.h"
#include "EventLoop.h"
#include "Socket.h"

namespace gnet {

class Acceptor : public NonCopyable {
private:
  int sockfd_;
  Ip4Addr addr_;

  // 最大监听数
  static const int kListen;
  static const int kInvalid;

public:
  Acceptor();
  ~Acceptor();

  // 创建监听套接字
  void Init();
  void Bind(const std::string &host, unsigned short port);
  void Listen();

  int GetSockFd() { return sockfd_; }
  Ip4Addr GetAddr() { return addr_; }

  // 创建新的连接，返回新的连接描述符
  int Accept(Ip4Addr &clientAddr);
};

} // namespace gnet

#endif // _ACCEPTOR_H