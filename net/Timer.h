#ifndef WEBSERVER_SRC_NET_TIMER_H_
#define WEBSERVER_SRC_NET_TIMER_H_

#include <sys/timerfd.h>
#include <cstdint>
#include <functional>
#include <atomic>
#include <memory>

#include <utils/noncopyable.h>

namespace gweb {

class Timer : noncopyable {
public:
  using TimerCallBack = std::function<void()>;
  using TimeType = int64_t ;

  Timer(TimeType now, const TimerCallBack &cb, TimeType interval);
  ~Timer() = default;

  void run() { cb_(); }

  TimeType getExpire() const { return expiration_; }
  uint64_t getSequence() const { return sequence_; }
  bool isRepeated() const { return repeated_; }

  bool isValid() const { return expiration_ >= Timer::now(); }
  TimeType invalid() const { return 0; }

  void restart();
  void remove();

  const static int kMsPerSec = 1000 * 1000;
  static TimeType now();
  static uint64_t getCreateNum();

private:
  TimeType expiration_; // 定时器生效的绝对时间
  const TimeType interval_; // 计时周期
  TimerCallBack cb_; // 超时回调函数
  bool repeated_; // 是否重复计时
  const uint64_t sequence_; // 序列号

  // 统计生成了多少个Timer
  static std::atomic<uint64_t> numCreated_;
};

// 封装了Timer，暴露给客户端使用
class TimerId {
public:
  TimerId() : sequence_(0) {}

  TimerId(const std::weak_ptr<Timer> &timer)
      : timer_(timer), sequence_(timer.lock() ? timer.lock()->getSequence() : 0) {}

  // 定时器队列可以使用私有成员变量
  friend class TimerQueue;
private:
  std::weak_ptr<Timer> timer_;
  uint64_t sequence_;
};

} // namespace web

#endif //WEBSERVER_SRC_NET_TIMER_H_
