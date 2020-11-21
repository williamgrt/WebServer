#ifndef WEBSERVER_SRC_NET_CHANNEL_H
#define WEBSERVER_SRC_NET_CHANNEL_H

#include <functional>
#include <memory>

namespace gweb {
using Callback = std::function<void()>;

class EventLoop;
class Epoll;

class Channel {
public:
  Channel(EventLoop *ev, int fd);
  ~Channel();

  EventLoop *getLoop() const { return loop_; }
  int fd() const { return fd_; }
  int32_t getEvents() const { return events_; }
  int32_t getRevents() const { return revents_; }
  int getState() const { return state_; }

  void setFd(int fd) { fd_ = fd; }
  void setEvents(uint32_t events) { events_ = events; }
  void setRevents(uint32_t revents) { revents_ = revents; }
  void setState(int state);
  bool isNoneEvent() { return events_ == kNoneEvent; }

  void setReadCallback(Callback &read) { readCb_ = read; }
  void setWriteCallback(Callback &write) { writeCb_ = write; }
  void setErrorCallback(Callback &error) { errorCb_ = error; }
  void setCloseCallback(Callback &close) { closeCb_ = close; }
  void setReadCallback(Callback &&read) { readCb_ = std::move(read); }
  void setWriteCallback(Callback &&write) { writeCb_ = std::move(write); }
  void setErrorCallback(Callback &&error) { errorCb_ = std::move(error); }
  void setCloseCallback(Callback &&close) { closeCb_ = std::move(close); }

  void setRead(bool label);
  void setWrite(bool label);
  void setError(bool label);
  void enableRead() { setRead(true); }
  void enableWrite() { setWrite(true); }
  bool isWriting() { return events_ & kWriteEvent; }
  void disableAll() {
    events_ = kNoneEvent;
    update();
  }

  void handleRead();
  void handleWrite();
  void handleError();
  void handleClose();
  void handleEvent();

  // 表示一个channel的状态
  static const int kNew; // 新建的channel
  static const int kAdded; // 添加到epoll监听队列中的channel
  static const int kDeleted; // 从epoll监听队列中删除的channel

  // channel事件
  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;
  static const int kErrorEvent;
  static const int kCloseEvent;

private:
  // 更新channel的状态，包括添加channel
  void update();

  EventLoop *loop_;
  int fd_;
  int32_t events_;
  int32_t revents_; // recieve events from poller.
  int state_;
  bool handlingEvent_;

  // 回调函数
  Callback readCb_;
  Callback writeCb_;
  Callback errorCb_;
  Callback closeCb_;
};

} // namespace web

#endif // WEBSERVER_SRC_NET_CHANNEL_H