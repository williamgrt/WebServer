#include "Acceptor.h"

namespace gnet {

const int Acceptor::kListen = 1024;
const int Acceptor::kInvalid = -1;

Acceptor::Acceptor() : sockfd_(Acceptor::kInvalid), addr_(Ip4Addr()) {}

Acceptor::~Acceptor() { socket::CloseSocket(sockfd_); }

void Acceptor::Init() {
  // 必须保证这个acceptor没有关联到一个socket上
  assert(sockfd_ == Acceptor::kInvalid);

  sockfd_ = socket::CreateTCPSocket();
  if (sockfd_ == -1) {
    error(1, errno, "create socket fail.\n");
  }
}

void Acceptor::Bind(const std::string &host, unsigned short port) {
  // 确保已经建立了连接
  assert(sockfd_ != -1);

  // 初始化socket地址
  addr_ = Ip4Addr(host, port);
  const sockaddr_in so_addr = addr_.getAddr();

  socket::SetNonBlock(sockfd_);
  socket::SetReuseAddr(sockfd_);

  sockfd_ = ::bind(sockfd_, (sockaddr *)(&addr_), sizeof(addr_));
  if (sockfd_) {
    error(1, errno, "bind error.");
  }
}

void Acceptor::Listen() {
  // 判断套接字已经打开
  assert(sockfd_ != -1);

  sockfd_ = ::listen(sockfd_, Acceptor::kListen);
  if (sockfd_ == -1) {
    error(1, errno, "listen error.");
  }
}

// TODO: designing better accrpt function
int Acceptor::Accept(Ip4Addr &clientAddr) {
  sockaddr_in addr = clientAddr.getAddr();
  socklen_t len;

  int connfd = ::accept(sockfd_, (sockaddr *)&addr, &len);
  if (connfd == -1) {
    error(1, errno, "accept error.");
  }

  return connfd;
}

} // namespace gnet