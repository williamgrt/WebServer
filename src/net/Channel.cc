#include "Channel.h"
#include "EventLoop.h"
#include "Utils.h"
#include <poll.h>

using namespace web;

// 表示一个channel的状态
const int Channel::kNew = -1; // 新建
const int Channel::kAdded = 1; // 添加到loop中
const int Channel::kDeleted = 2; // 从loop中删除

// 表示注册在channel上的事件类型
const int Channel::kNoneEvent = 0; // 没有事件
const int Channel::kReadEvent = POLLIN | POLLPRI; // 可读事件
const int Channel::kWriteEvent = POLLOUT; // 可写事件
const int Channel::kErrorEvent = POLLERR; // 错误事件

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(kNoneEvent), revents_(kNoneEvent),
      state_(kNew) {}

Channel::~Channel() {
  events_ = 0;
  fd_ = -1;
}

void Channel::setState(int state) {
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
    loop_->modifyChannel(this);
  }
}

void Channel::SetWrite(bool label) {
  if (label) {
    events_ |= kWriteEvent;
  } else {
    events_ &= ~kWriteEvent;
  }

  if (state_ == kAdded) {
    loop_->modifyChannel(this);
  }
}

void Channel::SetError(bool label) {
  if (label) {
    events_ |= kErrorEvent;
  } else {
    events_ &= ~kErrorEvent;
  }

  if (state_ == kAdded) {
    loop_->modifyChannel(this);
  }
}

void Channel::handleRead() {
  if (readCb_) {
    readCb_();
  }
}

void Channel::handleWrite() {
  if (writeCb_) {
    writeCb_();
  }
}

void Channel::handleError() {
  if (errorCb_) {
    errorCb_();
  }
}

// 根据接受事件类型执行不同的回调函数
void Channel::handleEvent() {
  if (revents_ & kReadEvent) {
    handleRead();
  }
  if (revents_ & kWriteEvent) {
    handleWrite();
  }
  if (revents_ & kErrorEvent) {
    handleError();
  }
}