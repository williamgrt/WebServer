#ifndef WEBSERVER_SRC_NET_SOCKET_H
#define WEBSERVER_SRC_NET_SOCKET_H

#include "Utils.h"
#include "../base/noncopyable.h"
#include <string>
#include <arpa/inet.h>

namespace web {

// Ipv4地址类
// 封装了sockaddr_in
class Ip4Addr {
private:
  sockaddr_in addr_;

public:
  Ip4Addr() = default;
  Ip4Addr(const std::string &host, unsigned short port);
  ~Ip4Addr();

  const sockaddr_in &getAddr() { return addr_; }
};

// 将socket封装为RAII类
// 目前仅支持Ipv4类地址，TCP协议连接
class Socket : noncopyable {
public:
  Socket();
  ~Socket();

  int fd() const { return sockfd_; }

  void bind(Ip4Addr &sockAddr);
  void listen(int backlog);
  int accept();

  void setNonBlocking();
  void setReuseAddr();
  void setNoDelay();
  void setKeepAlive();

private:
  int sockfd_;

  // 封装了socket处理函数及其错误处理部分
  void bindOpt(Ip4Addr &addr);
  void listenOpt(int backlog);
  int acceptOpt();
};

} // namespace web

#endif // WEBSERVER_SRC_NET_SOCKET_H