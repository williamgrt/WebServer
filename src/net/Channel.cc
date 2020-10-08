#include "Channel.h"
#include "EventLoop.h"
#include "Defs.h"
#include <poll.h>
#include <sys/epoll.h>

using namespace web;

// 表示一个channel的状态
const int Channel::kNew = -1; // 新建
const int Channel::kAdded = 1; // 添加到loop中
const int Channel::kDeleted = 2; // 从loop中删除

// 表示注册在channel上的事件类型
const int Channel::kNoneEvent = 0; // 没有事件
const int Channel::kReadEvent = POLLIN | POLLPRI ; // 可读事件
const int Channel::kWriteEvent = POLLOUT; // 可写事件
const int Channel::kErrorEvent = POLLERR | POLLNVAL; // 错误事件
const int Channel::kCloseEvent = POLLHUP;

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(kNoneEvent),
      revents_(kNoneEvent),
      state_(kNew),
      handlingEvent_(false) {}

Channel::~Channel() {
  // 正在处理事件的时候不能析构
  assert(!handlingEvent_);
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
  label ? events_ |= kReadEvent : events_ &= ~kReadEvent;
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

void Channel::handleClose() {
  if (closeCb_) {
    closeCb_();
  }
}

// 根据接受事件类型执行不同的回调函数
void Channel::handleEvent() {
  handlingEvent_ = true;
  // 可读事件
  if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
    handleRead();
  }
  // 可写事件
  if (revents_ & POLLOUT) {
    handleWrite();
  }
  // 错误事件
  if (revents_ & (POLLERR | POLLNVAL)) {
    handleError();
  }
  // 关闭连接
  if (revents_ & POLLHUP && !(revents_ & POLLIN)) {
    handleClose();
  }
  handlingEvent_ = false;
}

void Channel::update() {
  if (state_ == kAdded) {
    loop_->modify(this);
  } else {
    loop_->add(this);
  }
}
