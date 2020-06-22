#include <csignal>
#include "TcpServer.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "EventLoopThreadPool.h"

using namespace web;
using namespace std;
using namespace std::placeholders;

TcpServer::TcpServer(EventLoop *loop, const std::string &hostname, unsigned int port) :
    loop_(loop),
    acceptor_(new Acceptor(loop_, hostname, port)),
    name_("Connection-"),
    nextId_(1),
    started_(false),
    threadPool_(make_unique<EventLoopThreadPool>(loop_)) {
  acceptor_->setNewConnCallBack(std::bind(&TcpServer::newConnection, this, _1, _2));
  localAddr_ = acceptor_->getLocalAddr();
  // 忽略sigpipe信号
  handleSigpipe();
}

TcpServer::~TcpServer() {
  for (auto &element: connectionMap_) {
    // 关闭所有没有删除的连接
    TcpConnectionPtr conn = element.second;
    int n = connectionMap_.erase(element.first);
    assert(n == 1);
    // 将conn的生命周期延长到 connectDestroy 执行完毕
    conn->getLoop()->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
  }
}

void TcpServer::start() {
  if (!started_) {
    started_ = true;
  }
  threadPool_->start();
  // 为了保证所有的I/O操作都在 EventLoop 中执行
  loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
}

void TcpServer::newConnection(Socket &&socket, Ip4Addr peerAddr) {
  // 在 EventLoop 中执行
  loop_->assertInLoopThread();
  // 添加新的连接，给每个连接一个名字
  string connName = name_ + to_string(nextId_);
  nextId_++;
  // 创建新的连接，并保存指向连接的指针
  EventLoop *ioLoop = threadPool_->getNextLoop();
  TcpConnectionPtr newConn = make_shared<TcpConnection>(
      ioLoop,
      socket,
      localAddr_,
      peerAddr,
      connName
  );
  // 根据名称保存指向连接的指针
  connectionMap_[connName] = newConn;
  // 设定回调函数
  newConn->setConnectionCallback(connectionCallBack_);
  newConn->setMessageCallback(messageCallBack_);
  newConn->setCloseCallback(
      std::bind(&TcpServer::closeConnection, this, _1)
  );
  newConn->setWriteCompleteCallback(writeCompleteCallback_);
  // 建立连接（需要添加到TcpConnection自己的 EventLoop 中）
  ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, newConn));
}

void TcpServer::closeConnection(const TcpConnectionPtr &conn) {
  loop_->runInLoop(std::bind(&TcpServer::closeConnectionInLoop, this, conn));
}

void TcpServer::handleSigpipe() {
  ::signal(SIGPIPE, SIG_IGN);
}

void TcpServer::setThreadNum(int threadNum) {
  threadPool_->setThreadNum(threadNum);
}

void TcpServer::closeConnectionInLoop(const TcpConnectionPtr &conn) {
  loop_->assertInLoopThread();
  // 清除当前的连接
  int n = connectionMap_.erase(conn->name());
  assert(n == 1);
  // 必须要使用queueInLoop（为什么？）
  conn->getLoop()->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}
