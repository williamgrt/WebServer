#ifndef WEBSERVER_UTILS_TIMER_H_
#define WEBSERVER_UTILS_TIMER_H_

#include <cstdint>
#include <sys/time.h>

namespace gweb {
using TimeType = int64_t;
const static int kMsPerSec = 1000 * 1000;
// 封装时间处理的类
class Time {
 public:
  static TimeType now() {
    timeval tv{};
    ::gettimeofday(&tv, nullptr);
    return tv.tv_sec * kMsPerSec + tv.tv_usec;
  }
  // 现在的时间（以秒为单位）
  static TimeType nowSec() {
    timeval tv{};
    ::gettimeofday(&tv, nullptr);
    return tv.tv_sec;
  }

};

}

#endif //WEBSERVER_UTILS_TIMER_H_
