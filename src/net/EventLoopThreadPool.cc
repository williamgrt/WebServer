#include <cassert>
#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"

using namespace std;
using namespace web;

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop, int threadNum) :
    baseLoop_(baseLoop),
    threadNum_(threadNum),
    started_(false),
    pool_(),
    loops_(),
    nextLoopIndex_(0) {
}

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop) :
    baseLoop_(baseLoop),
    threadNum_(0),
    started_(false),
    nextLoopIndex_(0) {
}

EventLoop *EventLoopThreadPool::getNextLoop() {
  if (threadNum_ == 0) {
    return baseLoop_;
  } else {
    EventLoop *nextLoop = loops_[nextLoopIndex_];
    nextLoopIndex_++;
    if (static_cast<std::size_t>(nextLoopIndex_) >= loops_.size()) {
      nextLoopIndex_ = 0;
    }
    return nextLoop;
  }
}

void EventLoopThreadPool::setThreadNum(int threadNum) {
  if (!started_) {
    threadNum_ = threadNum;
  }
}

void EventLoopThreadPool::start() {
  assert(!started_);
  started_ = true;
  for (int i = 0; i < threadNum_; ++i) {
    EventLoopThread *newThread = new EventLoopThread();
    pool_.push_back(std::move(unique_ptr<EventLoopThread>(newThread)));
    loops_.push_back(newThread->start());
  }
}

EventLoopThreadPool::~EventLoopThreadPool() {

}

