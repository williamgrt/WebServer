#ifndef WEBSERVER_SRC_BASE_COUNTDOWNLATCH_H
#define WEBSERVER_SRC_BASE_COUNTDOWNLATCH_H

#include "Mutex.h"
#include "Condition.h"

namespace web {

class CountDownLatch : noncopyable {
public:
  explicit CountDownLatch(int count) :
      count_(count),
      mutex_(),
      cond_(mutex_) {}
  ~CountDownLatch() = default;

  void await();
  void countDown();

private:
  int count_;
  Mutex mutex_;
  Condition cond_;
};

}

#endif //WEBSERVER_SRC_BASE_COUNTDOWNLATCH_H
