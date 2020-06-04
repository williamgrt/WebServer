#include "CountDownLatch.h"

using namespace web;
using namespace std;

void CountDownLatch::await() {
  LockGuard guard(mutex_);
  cond_.waitIf([this]() { return this->count_ > 0;});
}

void CountDownLatch::countDown() {
  LockGuard guard(mutex_);
  count_--;
  if (count_ == 0) {
    cond_.notifyAll();
  }
}