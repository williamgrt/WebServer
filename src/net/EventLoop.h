#ifndef WEBSERVER_SRC_NET_EVENTLOOP_H
#define WEBSERVER_SRC_NET_EVENTLOOP_H

#include <memory>
#include <thread>
#include <vector>
#include "Timer.h"
#include "TimerQueue.h"

namespace web {
class Channel;
class EPoller;

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

  /* ------------------ 定时器操作相关的函数 ------------------ */
  TimerId runAfter(double delay, const Functor &cb);
  TimerId runEvery(double expiration, const Functor &cb);
  void cancel(TimerId timerId);

private:
  bool looping_;
  bool quit_;
  int eventCapacity_;
  std::unique_ptr<EPoller> poller_; // 执行 epoll 操作多路复用器
  std::thread::id ownerId_; // EventLoop 所处的线程（保证每个线程只有一个 EventLoop）

  std::mutex mutex_; // 保护任务队列
  std::vector<Functor> pendingFunctors_; // 任务队列

  int wakeupFd_; // 唤醒事件奋发器
  std::unique_ptr<Channel> wakeupChannel_;
  bool callingPendingFunctors_;

  std::unique_ptr<TimerQueue> timerQueue_; // 每个 eventloop 绑定一个定时器，处理定时事件

  void wakeup();
  void doPendingFunctors();
  void handleWakeup();
};

} // namespace web

#endif // WEBSERVER_SRC_NET_EVENTLOOP_H