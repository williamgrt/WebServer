#ifndef WEBSERVER_SRC_BASE_COUNTDOWNLATCH_H
#define WEBSERVER_SRC_BASE_COUNTDOWNLATCH_H

#include <mutex>
#include <condition_variable>

#include <gnet/utils/noncopyable.h>

namespace gweb {

class CountDownLatch : noncopyable {
public:
  explicit CountDownLatch(int count) :
      count_(count),
      mutex_(),
      cond_() {}
  ~CountDownLatch() = default;

  void await();
  void countDown();

private:
  int count_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

}

#endif //WEBSERVER_SRC_BASE_COUNTDOWNLATCH_H
