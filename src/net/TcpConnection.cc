#include <memory>
#include <iostream>
#include "TcpConnection.h"
#include "Channel.h"
#include "EventLoop.h"

using namespace web;
using namespace std;

TcpConnection::TcpConnection(EventLoop *loop,
                             Socket &socket,
                             Ip4Addr &localAddr,
                             Ip4Addr &peerAddr,
                             const string &name) :
    loop_(loop),
    name_(name),
    socket_(make_unique<Socket>(std::move(socket))),
    channel_(make_unique<Channel>(loop, socket_->fd())),
    localAddr_(localAddr),
    peerAddr_(peerAddr),
    state_(kConnecting),
    timeoutTimestamp_(nullptr) {
  // 设置连接属性
  setReuseAddr();
  setNoDelay();
  // 设置channel的回调函数
  channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
  channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
  channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
  // TODO: change standard output to log output
  cout << "TcpConnection create\n";
}

TcpConnection::~TcpConnection() {
  cout << "TcpConnection close " << name_ << " at " << this << " fd = " << channel_->fd() << "\n";
}

void TcpConnection::connectEstablished() {
  loop_->assertInLoopThread();
  assert(state_ == kConnecting);
  setState(kConnected);
  // 打开channel对应的监听事件
  channel_->enableRead();
  // 如果有，调用对应的连接建立函数
  if (connectionCallback_) {
    connectionCallback_(shared_from_this());
  }
}

void TcpConnection::send(const string &data) {
  if (state_ == kConnected) {
    loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, data));
  }
}

void TcpConnection::shutdown() {
  assert(state_ == kConnected);
  // TCP半关闭
  setState(kDisconnecting);
  // 必须要在I/O线程处理连接关闭的事情，保证优雅关闭
  loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
}

void TcpConnection::handleRead() {
  loop_->assertInLoopThread();
  int saveError = 0;
  int n = inputBuffer_.readFromFd(channel_->fd(), &saveError);
  if (n > 0) {
    // 对方发送了新的消息
    if (messageCallback_) {
      messageCallback_(shared_from_this(), &inputBuffer_);
    }
  } else if (n == 0) {
    // 对方已经关闭连接，read调用返回0
    // 服务端被动关闭
    handleClose();
  } else {
    // 读出错
    handleError();
  }
}

void TcpConnection::handleClose() {
  loop_->assertInLoopThread();
  // 通知绑定的poller不需要监听channel了
  channel_->disableAll();
  // 调用绑定的close回调函数（实际调用的是TcpServer::removeConnection）
  closeCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed() {
  loop_->assertInLoopThread();
  // 调整连接状态
  assert(state_ == kConnected || state_ == kDisconnecting);
  setState(kDisconnected);

  // 有可能直接调用本函数销毁连接，此时需要通知 poller 不需要监听
  channel_->disableAll();
  // 移除连接
  loop_->deleteChannel(channel_.get());
}

void TcpConnection::handleError() {
  // TODO: Add output log
}

void TcpConnection::handleWrite() {
  loop_->assertInLoopThread();

  if (channel_->isWriting()) {
    int n = ::write(channel_->fd(),
                    outputBuffer_.beginToRead(),
                    outputBuffer_.readableBytes());
    if (n > 0) {
      outputBuffer_.retrieve(n);
      // 没有新的可读数据了
      if (outputBuffer_.readableBytes() == 0) {
        // 关闭写事件监听
        channel_->setWrite(false);
        if (state_ == kDisconnecting) {
          shutdownInLoop();
        }
      } else {
        // TODO: Add log codes
      }
    } else {
      // TODO: Add log codes
    }
  } else {
    // TODO: Add log codes
  }
}

void TcpConnection::sendInLoop(const string &data) {
  loop_->assertInLoopThread();
  ssize_t nWrote = 0;
  // 如果当前的 channel 没有执行监听任务，并且 outputBuffer 中没有需要发送的数据，可以尝试直接发送
  // 主要的原因是可以加快发送的效率
  // 如果所有的数据都是通过
  if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0) {
    nWrote = ::write(channel_->fd(), data.data(), data.size());
    if (nWrote >= 0) {
      // TODO: Add codes to handle this problem
    } else {
      // 写出错
      nWrote = 0;
      // TODO:
      if (errno != EWOULDBLOCK) {
        if (errno == EPIPE || errno == ECONNRESET) {

        }
      }
    }
  }

  // 数据没有发完，需要写入到buffer中
  assert(nWrote >= 0);
  if (static_cast<std::size_t>(nWrote) < data.size()) {
    outputBuffer_.append(data.data() + nWrote, data.size() - nWrote);
    // 打开写监听
    if (!channel_->isWriting()) {
      channel_->enableWrite();
    }
  }
}

void TcpConnection::shutdownInLoop() {
  loop_->assertInLoopThread();
  // 如果服务器还需要监听写事件，就不能关闭写端，保证优雅关闭
  if (!channel_->isWriting()) {
    // 如果没有监听写事件（服务端无写的需求）
    // 服务端关闭写端套接字
    socket_->shutdownWrite();
  }
}

void TcpConnection::forceClose() {
  if (state_ == kConnected || state_ == kDisconnecting) {
    loop_->runInLoop(std::bind(&TcpConnection::forceCloseInLoop, this));
  }
}

void TcpConnection::delayClose(int seconds) {
  if (state_ == kConnected || state_ == kDisconnecting) {
    setState(kDisconnecting);
    loop_->runAfter(seconds, std::bind(&TcpConnection::handleClose, this));
  }
}

void TcpConnection::forceCloseInLoop() {
  loop_->assertInLoopThread();
  if (state_ == kConnected || state_ == kDisconnecting) {
    handleClose();
  }
}
