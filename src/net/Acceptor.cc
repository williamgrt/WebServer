#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"

using namespace web;
using namespace std;

Acceptor::Acceptor(EventLoop *loop, const std::string &hostname, unsigned short port)
    : loop_(loop),
      socket_(),
      addr_(hostname, port),
      socketChannel_(make_unique<Channel>(loop_, socket_.fd())),
      listening_(false) {
  // 在构造函数中完成地址绑定
  socket_.bind(addr_);
  // 让socket变为非阻塞的socket
  socket_.setNonBlocking();
  // 让 acceptor 可以进行回调
  socketChannel_->setReadCallback(std::bind(&Acceptor::handleRead, this));
  socketChannel_->enableRead();
}

void Acceptor::listen() {
  assert(!listening_);
  listening_ = true;
  socket_.listen(1024);
}

// acceptor接收到新连接后调用的函数
void Acceptor::handleRead() {
  loop_->assertInLoopThread();

  Ip4Addr peerAddr{};
  // 一次只 accept 一次
  int conn = socket_.accept(peerAddr);
  Socket newConnSocket(conn);
  if (newConnCallBack_) {
    newConnCallBack_(std::move(newConnSocket), peerAddr);
  } else {
    // 没有对应的处理函数，直接关闭套接字连接
    newConnSocket.close();
  }
}

