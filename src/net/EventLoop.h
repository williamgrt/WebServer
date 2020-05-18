#ifndef WEBSERVER_SRC_NET_EVENTLOOP_H
#define WEBSERVER_SRC_NET_EVENTLOOP_H

#include <memory>
#include <thread>
#include <vector>
#include "../base/Mutex.h"

namespace web {
class Channel;
class EPoller;

// 单线程版本的EventLoop
class EventLoop {

public:
  using Functor = std::function<void()>;

  EventLoop();
  ~EventLoop();

  /* ------------------ 对 channel 执行的操作 ------------------ */
  void addChannel(Channel *channel);
  void modifyChannel(Channel *channel);
  void deleteChannel(Channel *channel);

  /* ------------------ EventLoop 运行函数 ------------------ */
  void loop();
  void quit();

  /* ------------------ 跨线程执行的函数 ------------------ */
  void runInLoop(const Functor &cb);
  void queueInLoop(const Functor &cb);

  void assertInLoopThread() const;
  bool isInLoopThread() const;

private:
  bool looping_;
  bool quit_;
  int eventCapacity_;
  std::unique_ptr<EPoller> poller_;
  pid_t ownerId_; // EventLoop 所处的线程

  Mutex mutex_;
  std::vector<Functor> pendingFunctors_;

  int wakeupFd_;
  std::unique_ptr<Channel> wakeupChannel_;
  bool callingPendingFunctors_;

  void wakeup();
  void doPendingFunctors();
  void handleWakeup();
};

} // namespace web

#endif // WEBSERVER_SRC_NET_EVENTLOOP_H