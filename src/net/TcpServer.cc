#include "TcpServer.h"

namespace web {

TcpServer::TcpServer(EventLoop *ev)
    : ev_(ev), acceptor_(new Acceptor()), state_(-1) {}

types::TcpServerPtr TcpServer::Start(const std::string &host,
                                     unsigned short port) {
  assert(state_ == -1);
  acceptor_->Bind(host, port);
}

} // namespace web
