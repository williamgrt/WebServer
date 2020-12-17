#ifndef WEBSERVER_SRC_NET_HEARTBEATTCPSERVER_H
#define WEBSERVER_SRC_NET_HEARTBEATTCPSERVER_H

#include <unordered_set>
#include <list>

#include <utils/noncopyable.h>
#include "TcpServer.h"
#include "Defs.h"

namespace gweb {

class HeartBeatTcpServer : noncopyable {
 public:
  HeartBeatTcpServer(EventLoop *loop, const std::string &hostname, unsigned int port, int aliveSec_);
  ~HeartBeatTcpServer();

  void setThreadNum(int threadNum) {
    server_.setReactorNum(threadNum);
  }

  void start() {
    server_.start();
  }

  void setConnectionCallback(const ConnectionCallback &cb) {
    connectionCallback_ = cb;
  }

  void setMessageCallback(const MessageCallback &cb) {
    messageCallback_ = cb;
  }

  void setWriteCompleteCallback(const WriteCompleteCallback &cb) {
    writeCompleteCallback_ = cb;
  }

  EventLoop *getLoop() {
    return loop_;
  }

 private:
  // 回调函数
  void onConnect(const TcpConnPtr &conn);
  void onMessage(const TcpConnPtr &conn, Buffer *buffer);


  void onTimeout();

  EventLoop *loop_;
  TcpServer server_;
  int keepaliveTimeout_; // 长连接的过期时间

  // 回调函数
  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  WriteCompleteCallback writeCompleteCallback_;

  // 过期时间戳
  struct Timestamp {
   public:
    Timestamp(const TcpConnPtr &conn) :
        conn_(conn) {
    }

    ~Timestamp() {
      // 在析构函数中关闭超时连接
      // 使用weak_ptr监视connection
      TcpConnPtr conn = conn_.lock();
      // 如果当前的连接没有被析构
      if (conn) {
        conn->forceClose();
      }
    }

    void setWeakPtr(const std::weak_ptr<Timestamp> &ptr) {
      ptr_ = ptr;
    }

    std::weak_ptr<Timestamp> getWeakPtr() {
      return ptr_;
    };

    std::weak_ptr<TcpConnection> conn_;
    std::weak_ptr<Timestamp> ptr_;
  };

  using TimestampPtr = std::shared_ptr<Timestamp>;
  using TimestampWeakPtr = std::weak_ptr<Timestamp>;
  using TimestampSet = std::unordered_set<TimestampPtr>;
  using Bucket = std::list<TimestampSet>;

  // 添加一个时间戳
  void addTimestamp(const TimestampPtr &timestamp);

  // 详细的设计思路参考文档
  Bucket bucket_;
};

}

#endif //WEBSERVER_SRC_NET_HEARTBEATTCPSERVER_H
