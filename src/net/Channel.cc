#include "Channel.h"
#include "EventLoop.h"
#include "Utils.h"
#include <sys/epoll.h>

using namespace web;

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(kNoneEvent), revents_(kNoneEvent),
      state_(kNew) {}

Channel::~Channel() {
  events_ = 0;
  fd_ = -1;
}

void Channel::SetState(int state) {
  assert(state == kNew || state == kAdded || state == kDeleted);
  state_ = state;
}

void Channel::SetRead(bool label) {
  if (label) {
    events_ |= kReadEvent;
  } else {
    events_ &= ~kReadEvent;
  }

  if (state_ == kAdded) {
    loop_->ModifyChannel(this);
  }
}

void Channel::SetWrite(bool label) {
  if (label) {
    events_ |= kWriteEvent;
  } else {
    events_ &= ~kWriteEvent;
  }

  if (state_ == kAdded) {
    loop_->ModifyChannel(this);
  }
}

void Channel::SetError(bool label) {
  if (label) {
    events_ |= kErrorEvent;
  } else {
    events_ &= ~kErrorEvent;
  }

  if (state_ == kAdded) {
    loop_->ModifyChannel(this);
  }
}

void Channel::HandleRead() {
  if (readCb_) {
    readCb_();
  }
}

void Channel::HandleWrite() {
  if (writeCb_) {
    writeCb_();
  }
}

void Channel::HandleError() {
  if (errorCb_) {
    errorCb_();
  }
}

// 根据接受事件执行不同的回调函数
void Channel::HandleEvent() {
  if (revents_ & kReadEvent) {
    HandleRead();
  }
  if (revents_ & kWriteEvent) {
    HandleWrite();
  }
  if (revents_ & kErrorEvent) {
    HandleError();
  }
}