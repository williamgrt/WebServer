#include "Socket.h"
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <cstring>

using namespace web;
using namespace std;

Ip4Addr::Ip4Addr(const std::string &host, unsigned short port) {
  bzero(&addr_, sizeof(sockaddr_in));
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  if (host.empty()) {
    // 没有指定IP地址
    addr_.sin_addr.s_addr = ::htonl(INADDR_ANY);
  } else {
    // 指定IP地址
    int r = inet_pton(AF_INET, host.c_str(), &(addr_.sin_addr));
    if (r == 0) {
      error(1, errno, "invalid ip address");
      abort();
    } else if (r == -1) {
      error(1, errno, "converting ip address error.");
      abort();
    }
  }
}

Ip4Addr::~Ip4Addr() {}

Socket::Socket() {
  sockfd_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd_ == -1) {
    error(1, errno, "create socket error.");
    abort();
  }
}

Socket::~Socket() {
  ::close(sockfd_);
}

void Socket::bind(Ip4Addr &sockAddr) {
  assert(sockfd_ != -1);
  bindOpt(sockAddr);
}

void Socket::listen(int backlog) {
  assert(sockfd_ != -1);
  listenOpt(backlog);
}

int Socket::accept() {
  return acceptOpt();
}

void Socket::setNoDelay() {
  int nodelay = 1;
  ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, (const char *) &nodelay, sizeof(nodelay));
}

void Socket::setNonBlocking() {
  auto flag = ::fcntl(sockfd_, F_GETFL, 0);
  ::fcntl(sockfd_, F_SETFL, flag | O_NONBLOCK);
}

void Socket::setReuseAddr() {
  int arg = 1;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, (const char *) &arg, sizeof(arg));
}

void Socket::setKeepAlive() {
  int arg = 1;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, (const char *) &arg, sizeof(arg));
}

void Socket::bindOpt(Ip4Addr &sockAddr) {
  auto addr = sockAddr.getAddr();

  // 设置socket选项
  setNoDelay();
  setNonBlocking();
  setKeepAlive();
  setReuseAddr();

  sockfd_ = ::bind(sockfd_, (sockaddr *) &addr, sizeof(addr));
  if (sockfd_ == -1) {
    error(1, errno, "socket bind error.");
    abort();
  }
}

int Socket::acceptOpt() {
  Ip4Addr clientAddr;
  auto addr = clientAddr.getAddr();
  socklen_t addrLen = 0;
  int connfd = ::accept(sockfd_, (sockaddr *) &addr, &addrLen);
  if (connfd == -1) {
    error(1, errno, "accept error.");
    abort();
  }

  return connfd;
}

void Socket::listenOpt(int backlog) {
  sockfd_ = ::listen(sockfd_, backlog);
  if (sockfd_ == -1) {
    error(1, errno, "socket listen error.");
    abort();
  }
}
