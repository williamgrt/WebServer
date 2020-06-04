/***********
 * @brief 封装 socket 描述符和 sockaddr_in 地址
 */

#ifndef WEBSERVER_SRC_NET_SOCKET_H
#define WEBSERVER_SRC_NET_SOCKET_H

#include "Utils.h"
#include "../base/noncopyable.h"
#include <string>
#include <arpa/inet.h>

namespace web {

class Ip4Addr {
public:
  Ip4Addr() = default;
  Ip4Addr(const std::string &host, unsigned short port);
  ~Ip4Addr();

  const sockaddr_in &getAddr() { return addr_; }

private:
  sockaddr_in addr_;
};

// 将socket封装为RAII类
// 目前仅支持Ipv4类地址，TCP协议连接
class Socket : noncopyable {
public:
  Socket();
  explicit Socket(int sockfd);
  ~Socket();

  int fd() const { return sockfd_; }
  bool isInvalid() { return sockfd_ < 0; }
  void setInvalid() { sockfd_ = -1; }

  /* ------------------ socket 操作函数 ------------------ */
  void bind(Ip4Addr &sockAddr);
  void listen(int backlog);
  int accept(Ip4Addr &peer);
  void close();

  void setNonBlocking();
  void setReuseAddr();
  void setNoDelay();
  void setKeepAlive();

private:
  int sockfd_; // 套接字描述符

  // 封装了socket处理函数及其错误处理部分
  void bindOpt(Ip4Addr &addr);
  void listenOpt(int backlog);
  int acceptOpt(Ip4Addr &peer);
  void closeOpt();
};

} // namespace web

#endif // WEBSERVER_SRC_NET_SOCKET_H