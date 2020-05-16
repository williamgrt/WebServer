#include "Timer.h"
#include <time.h>

using namespace web;
using namespace std;

atomic<uint64_t> Timer::numCreated_(0);
Timer::Timer(TimeType now, const TimerCallBack &cb, TimeType interval = 0) : expiration_(now),
                                                                             interval_(interval),
                                                                             cb_(cb),
                                                                             repeated_(interval_ > 0),
                                                                             sequence_(numCreated_) {
  numCreated_++;
}

/*
 * 获取当前时间
 */
Timer::TimeType Timer::now() {
  timespec ts = timespec();
  clock_gettime(CLOCK_REALTIME, &ts);
  // 目前定时器仅支持秒级定时
  // 未来可能会添加毫秒级定时的支持
  return ts.tv_sec;
}

uint64_t Timer::getCreateNum() {
  return static_cast<uint64_t>(Timer::numCreated_);
}

/*
 * 只针对具有周期性的计时器采取重启功能
 */
void Timer::restart() {
  if (repeated_) {
    expiration_ = now() + interval_;
  } else {
    expiration_ = 0;
  }
}
