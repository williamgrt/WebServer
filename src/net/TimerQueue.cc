#include "TimerQueue.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Utils.h"
#include <sys/timerfd.h>

using namespace web;
using namespace std;

TimerQueue::TimerQueue(EventLoop *loop)
    : loop_(loop), timerfd_(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)),
      timerfdChannel_(new Channel(loop_, timerfd_)) {
  timerfdChannel_->setReadCallBack(std::bind(&TimerQueue::handleRead, this));
  timerfdChannel_->enableRead();
}

TimerQueue::~TimerQueue() {
  ::close(timerfd_);
}

TimerId TimerQueue::addTimer(Timer::TimeType now, Timer::TimerCallBack cb, Timer::TimeType interval) {
  TimerPtr newTimer = make_shared<Timer>(now, cb, interval);

  TimerId timerId(newTimer);
  return timerId;
}

