#include "Timer.h"
#include <time.h>
#include <sys/time.h>

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
  timeval tv{};
  ::gettimeofday(&tv, nullptr);
  return tv.tv_sec * kMsPerSec + tv.tv_usec;
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

void Timer::remove() {
  // 直接将定时器的超时时间设置为0，表示删除了这个定时器
  expiration_ = 0;
}
