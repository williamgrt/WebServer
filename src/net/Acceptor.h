#ifndef WEBSERVER_SRC_NET_ACCEPTOR_H
#define WEBSERVER_SRC_NET_ACCEPTOR_H

#include "Socket.h"
#include "Channel.h"
#include <string>
#include <memory>

namespace web {
class EventLoop;

class Acceptor {
public:
  using NewConnCallBack = std::function<void(Socket &&, Ip4Addr)>;

  Acceptor(EventLoop *loop, const std::string &hostname, unsigned short port);
  ~Acceptor() {}

  void setNewConnCallBack(const NewConnCallBack &cb) { newConnCallBack_ = cb; }

  void listen();
  bool isListening() const { return listening_; }
  Ip4Addr getLocalAddr() { return addr_; }

private:
  void handleRead();

  EventLoop *loop_;
  Socket socket_;
  Ip4Addr addr_;
  std::unique_ptr<Channel> socketChannel_;
  NewConnCallBack newConnCallBack_; // accept 以后创建新连接的函数
  bool listening_;
};

}

#endif //WEBSERVER_SRC_NET_ACCEPTOR_H
