#ifndef WEBSERVER_SRC_NET_TIMERQUEUE_H
#define WEBSERVER_SRC_NET_TIMERQUEUE_H

#include <net/Defs.h>
#include <utils/noncopyable.h>
#include <net/Timer.h>

#include <memory>
#include <queue>
#include <vector>
#include <set>
#include <mutex>

namespace gweb {
class EventLoop;
class Channel;

class TimerQueue : noncopyable {
public:
  explicit TimerQueue(EventLoop *loop);
  ~TimerQueue();

  // 往定时队列中添加新的定时器
  TimerId addTimer(Timer::TimeType now, Timer::TimerCallBack cb, Timer::TimeType interval);
  // 取消一个特定的定时器
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

  std::mutex mutex_; // 保护定时器队列
  TimerList timers_; // 所有的定时器

  std::vector<Timer> activeTimers_;

};

} // namespace web

#endif //WEBSERVER_SRC_NET_TIMERQUEUE_H
