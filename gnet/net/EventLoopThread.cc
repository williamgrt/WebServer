#include "EventLoopThread.h"
#include "EventLoop.h"
#include <cassert>

using namespace gweb;
using namespace std;

EventLoopThread::EventLoopThread() :
    loop_(nullptr),
    thread_(),
    mutex_(),
    cond_() {}

EventLoopThread::~EventLoopThread() {
  if (loop_ != nullptr) {
    thread_.join();
  }
}

EventLoop *EventLoopThread::start() {
  assert(loop_ == nullptr);
  thread_ = thread(&EventLoopThread::threadFunc, this);

  unique_lock<mutex> lock(mutex_);
  while (loop_ == nullptr) {
    cond_.wait(lock);
  }
  return loop_;
}

void EventLoopThread::threadFunc() {
  EventLoop loop;
  {
    // 保证EventLoop已经被创建
    lock_guard<mutex> guard(mutex_);
    loop_ = &loop;
    cond_.notify_one();
  }
  // 一直循环
  loop_->loop();
  // 循环结束
  lock_guard<mutex> guard(mutex_);
  loop_ = nullptr;
}