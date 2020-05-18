//
// Created by 郭睿韬 on 2020/5/13.
//

#ifndef WEBSERVER_SRC_NET_TIMERQUEUE_H
#define WEBSERVER_SRC_NET_TIMERQUEUE_H

#include "../base/noncopyable.h"
#include "Timer.h"
#include <memory>
#include <queue>
#include <vector>

namespace web {
class EventLoop;
class Channel;

class TimerQueue : noncopyable {
public:
  explicit TimerQueue(EventLoop *loop);
  ~TimerQueue();

  TimerId addTimer(Timer::TimeType now, Timer::TimerCallBack cb, Timer::TimeType interval);
  void cancel(TimerId timerId);

private:
  using TimerPtr = std::shared_ptr<Timer>;
  using Entry = std::pair<Timer::TimeType, TimerPtr>;
  using TimerList = std::priority_queue<Entry>;

  // 处理timerfd的到期事件
  void handleRead();

  EventLoop *loop_;
  const int timerfd_;
  std::unique_ptr<Channel> timerfdChannel_;
  std::vector<TimerPtr> allTimers_;
};

} // namespace web

#endif //WEBSERVER_SRC_NET_TIMERQUEUE_H
