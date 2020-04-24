#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "../base/noncopyable.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include <memory>
#include <string>

namespace web {

class Ip4Addr;
class Channel;
class EventLoop;

class Acceptor : public noncopyable {
private:
  // acceptor和socket的生命周期一致
  int sockfd_;
  Ip4Addr addr_;
  EventLoop *loop_;
  std::unique_ptr<Channel> listenChannel_; // 监听套接字

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

} // namespace web

#endif // _ACCEPTOR_H