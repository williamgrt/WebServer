#include "Socket.h"

namespace gnet {
namespace socket {

int CreateTCPSocket() { return ::socket(AF_INET, SOCK_STREAM, 0); }

int CreateUDPSocket() { return ::socket(AF_INET, SOCK_DGRAM, 0); }

void CloseSocket(int &sockfd) {
  if (sockfd != -1) {
    ::close(sockfd);
    sockfd = -1;
  }
}

void SetNonBlocking(int &sockfd) {
  int flag = ::fcntl(sockfd, F_GETFL, 0);
  flag = ::fcntl(sockfd, F_SETFL, flag | O_NONBLOCK);
}

} // namespace socket

Ip4Addr::Ip4Addr() {}

Ip4Addr::Ip4Addr(const std::string &host, unsigned short port) {
  bzero(&addr_, sizeof(sockaddr_in));
  addr_.sin_family = AF_INET;
  addr_.sin_port = ::htons(port);
  if (host.empty()) {
    // 没有指定IP地址
    addr_.sin_addr.s_addr = ::htonl(INADDR_ANY);
  } else {
    // 指定IP地址
    int r = ::inet_pton(AF_INET, host.c_str(), &(addr_.sin_addr));
    if (r == 0) {
      error(1, errno, "invalid ip address");
    } else if (r == -1) {
      error(1, errno, "converting ip address error.");
    }
  }
}

Ip4Addr::~Ip4Addr() {}

} // namespace gnet
