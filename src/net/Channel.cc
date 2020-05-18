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
  // 不要关闭文件描述符
  // 文件描述符的关闭由持有该结构的类实现
  fd_ = -1;
}

void Channel::setState(int state) {
  assert(state == kNew || state == kAdded || state == kDeleted);
  state_ = state;
}

void Channel::setRead(bool label) {
  label ? events_ |= kReadEvent : events_ &= kReadEvent;
  update();
}

void Channel::setWrite(bool label) {
  label ? events_ |= kWriteEvent : events_ &= ~kWriteEvent;
  update();
}

void Channel::setError(bool label) {
  label ? events_ |= kErrorEvent : events_ &= ~kErrorEvent;
  update();
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
  // 可读事件
  if (revents_ & kReadEvent) {
    handleRead();
  }

  // 可写事件
  if (revents_ & kWriteEvent) {
    handleWrite();
  }

  // 错误事件
  if (revents_ & kErrorEvent) {
    handleError();
  }
}

void Channel::update() {
  if (state_ == kAdded) {
    loop_->modifyChannel(this);
  } else {
    loop_->addChannel(this);
  }
}
