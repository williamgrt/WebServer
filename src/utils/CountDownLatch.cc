#include "CountDownLatch.h"

using namespace web;
using namespace std;

void CountDownLatch::await() {
  unique_lock<mutex> lock(mutex_);
  while (count_ > 0) {
    cond_.wait(lock);
  }
}

void CountDownLatch::countDown() {
  lock_guard<mutex> guard(mutex_);
  count_--;
  if (count_ == 0) {
    cond_.notify_all();
  }
}