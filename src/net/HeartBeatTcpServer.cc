#include "HeartBeatTcpServer.h"
#include "EventLoop.h"

using namespace std;
using namespace web;
using namespace std::placeholders;

HeartBeatTcpServer::HeartBeatTcpServer(EventLoop *loop, const string &hostname, unsigned int port, int aliveSec_) :
    server_(loop, hostname, port),
    keepaliveTimeout_(aliveSec_) {
  // 每秒调用一次timeout处理程序
  server_.getLoop()->runEvery(1.0, std::bind(&HeartBeatTcpServer::onTimeout, this));
  server_.setConnectionCallback(std::bind(&HeartBeatTcpServer::onConnect, this, _1));
  server_.setMessageCallback(std::bind(&HeartBeatTcpServer::onMessage, this, _1, _2));
  server_.setWriteCompleteCallback(writeCompleteCallback_);
  //
  bucket_.resize(keepaliveTimeout_);
}

HeartBeatTcpServer::~HeartBeatTcpServer() {

}

void HeartBeatTcpServer::onConnect(const TcpConnectionPtr &conn) {
  if (connectionCallback_) {
    connectionCallback_(conn);
  }
  if (conn->connected()) {
    // 添加一个过期时间标志
    TimestampPtr timestampPtr(new Timestamp(conn));
    TimestampWeakPtr weakPtr(timestampPtr);
    timestampPtr->setWeakPtr(weakPtr);
    // 把新生成的指针保存到连接中
    // 所有新生成的智能指针都使用这个指针初始化
    // 必须保存一份智能指针的副本，如果使用的是原始指针，会导致重复释放内存的问题
    // 同时不能使用shared_ptr，因为会有循环引用的问题
    // 所以要在timestamp里面保存weak_ptr用于获取存储在服务器中的时间戳
    conn->setTimestamp(static_cast<void *>(timestampPtr.get()));
    // 添加时间戳
    loop_->runInLoop(std::bind(&HeartBeatTcpServer::addTimestamp, this, timestampPtr));
  } else {
    // TODO:
  }
}

void HeartBeatTcpServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer) {
  if (messageCallback_) {
    messageCallback_(conn, buffer);
  }
  Timestamp *timestamp = static_cast<Timestamp *>(conn->getTimestamp());
  assert(timestamp != nullptr);
  // 获得指向时间戳条目的智能指针
  TimestampWeakPtr timestampWPtr = timestamp->getWeakPtr();
  TimestampPtr timestampPtr = timestampWPtr.lock();
  if (timestampPtr) {
    // 重新添加到时间戳集合中
    loop_->runInLoop(std::bind(&HeartBeatTcpServer::addTimestamp, this, timestampPtr));
  }
}

void HeartBeatTcpServer::onTimeout() {
  // 清除所有最老的时间戳
  bucket_.pop_front();
  // 放入一个新的桶用于添加最新的时间戳
  bucket_.push_back(TimestampSet());
}

void HeartBeatTcpServer::addTimestamp(const HeartBeatTcpServer::TimestampPtr &timestamp) {
  bucket_.back().insert(timestamp);
}
