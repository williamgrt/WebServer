//
// Created by 郭睿韬 on 2020/5/13.
//

#include "TimerQueue.h"
#include "EventLoop.h"
#include "Channel.h"
#include <sys/timerfd.h>

using namespace web;
using namespace std;

TimerQueue::TimerQueue(EventLoop *loop)
    : loop_(loop), timerfd_(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)),
      timerfdChannel_(new Channel(loop_, timerfd_)) {
}


