#ifndef WEBSERVER_SRC_NET_TCPCONNECTION_H
#define WEBSERVER_SRC_NET_TCPCONNECTION_H

#include <string>
#include "Utils.h"
#include "Socket.h"
#include "Buffer.h"

namespace web {
// TCP连接类
class Ip4Addr;
class EventLoop;
class Channel;

class TcpConnection : noncopyable,
                      public std::enable_shared_from_this<TcpConnection> {
public:
  TcpConnection(EventLoop *loop, Socket &socket, Ip4Addr &localAddr, Ip4Addr &peerAddr, const std::string &name);
  ~TcpConnection();

  const std::string name() { return name_; }
  bool connected() { return state_ == kConnected; }
  EventLoop *getLoop() { return loop_; }
  Ip4Addr &getLocalAddr() { return localAddr_; }
  Ip4Addr &getPeerAddr() { return peerAddr_; }

  void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
  void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
  void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }
  void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }

  void connectEstablished();
  void connectDestroyed();

  /*****
   * @brief 发送消息给连接的对方，实际的写操作需要到I/O线程中执行。
   * @param data 待发送的消息。
   */
  void send(const std::string &data);

  /* ---------- 连接关闭操作 ---------- */

  /*****
   * @brief 主动关闭写端，实现优雅断开，实际的I/O操作需要到线程中执行。
   */
  void shutdown();
  void forceClose();
  void delayClose(int seconds);

private:
  void handleRead();
  void handleWrite();
  void handleClose();
  void handleError();

  void sendInLoop(const std::string &data);
  void shutdownInLoop();
  void forceCloseInLoop();
  // 连接状态
  enum State {
    kConnecting, // 正在创建连接
    kConnected, // 连接已完成
    kDisconnected, // 半关闭（服务端主动关闭）
    kDisconnecting // 被动关闭
  };
  void setState(State s) { state_ = s; }

  EventLoop *loop_;
  const std::string name_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  CloseCallback closeCallback_;
  WriteCompleteCallback writeCompleteCallback_;
  Ip4Addr localAddr_;
  Ip4Addr peerAddr_;
  State state_;
  Buffer inputBuffer_;
  Buffer outputBuffer_;
};
}

#endif //WEBSERVER_SRC_NET_TCPCONNECTION_H
