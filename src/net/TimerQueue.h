//
// Created by 郭睿韬 on 2020/5/13.
//

#ifndef WEBSERVER_SRC_NET_TIMERQUEUE_H
#define WEBSERVER_SRC_NET_TIMERQUEUE_H

#include "../base/noncopyable.h"
#include "Timer.h"
#include "../base/Mutex.h"

#include <memory>
#include <queue>
#include <vector>
#include <set>


namespace web {
class EventLoop;
class Channel;
class Mutex;

class TimerQueue : noncopyable {
public:
  explicit TimerQueue(EventLoop *loop);
  ~TimerQueue();

  TimerId addTimer(Timer::TimeType now, Timer::TimerCallBack cb, Timer::TimeType interval);

  void cancel(TimerId timerId);

private:
  using TimerPtr = std::shared_ptr<Timer>;
  using Entry = std::pair<Timer::TimeType, TimerPtr>;
  using TimerList = std::set<Entry>;

  void handleRead();
  std::vector<Entry> getExpiredTimer(Timer::TimeType now);
  // 插入新的计时器
  bool addTimer(const TimerPtr &timer);
  void reset(std::vector<Entry> &expired);

  EventLoop *loop_;
  const int timerfd_;
  std::unique_ptr<Channel> timerfdChannel_;

  Mutex mutex_; // 保护定时器队列
  TimerList timers_; // 所有的定时器

  std::vector<Timer> activeTimers_;

};

} // namespace web

#endif //WEBSERVER_SRC_NET_TIMERQUEUE_H
