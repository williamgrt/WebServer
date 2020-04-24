#ifndef _SOCKET_H
#define _SOCKET_H

#include "Utils.h"
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <sys/socket.h>

namespace web {
namespace socket {
// 创建和销毁socket
int CreateTCPSocket();
int CreateUDPSocket();
void CloseSocket(int &sockfd);

// 对socket进行设置
void SetNonBlocking(int &sockfd);
void SetReuseAddr(int &sockfd);
void SetNoDelay(int &sockfd); // 禁用Nagel算法

} // namespace socket

// Ipv4地址类
class Ip4Addr {
private:
  sockaddr_in addr_;

public:
  Ip4Addr();
  Ip4Addr(const std::string &host, unsigned short port);
  ~Ip4Addr();

  const sockaddr_in &getAddr() { return addr_; }
};

} // namespace web

#endif // _SOCKET_H