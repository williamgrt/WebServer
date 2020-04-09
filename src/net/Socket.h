#ifndef _SOCKET_H
#define _SOCKET_H

#include "Utils.h"
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <sys/socket.h>

namespace gnet {
namespace socket {
// 创建和销毁socket
int CreateTCPSocket();
int CreateUDPSocket();
void CloseSocket(int &sockfd);

int SetNonBlock(int &sockfd);

// 设置socket属性
void SetReuseAddr(int &sockfd);

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

} // namespace gnet

#endif // _SOCKET_H