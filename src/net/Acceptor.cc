#include "Acceptor.h"

namespace gnet {

Acceptor::Acceptor(EventLoop *ev) : ev_(ev), sockfd_(-1), addr_(Ip4Addr()) {}

Acceptor::~Acceptor() {
  socket::CloseSocket(sockfd_);
  ev_ = nullptr;
}

void Acceptor::Init() {
  // 必须保证这个acceptor没有关联到一个socket上
  assert(sockfd_ == -1);

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

void Acceptor::Listen(int maxWaitNum) {
  // 判断套接字已经打开
  assert(sockfd_ != -1);

  sockfd_ = ::listen(sockfd_, maxWaitNum);
  if (sockfd_ == -1) {
    error(1, errno, "listen error.");
  }
}

} // namespace gnet