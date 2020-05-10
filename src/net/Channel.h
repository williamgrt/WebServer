#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <functional>
#include <memory>

namespace web {
using CallBack = std::function<void()>;

class EventLoop;
class EPoller;

class Channel {
 public:
  Channel(EventLoop *ev, int fd);
  Channel(EventLoop *ev);
  ~Channel();

  EventLoop *getLoop() const { return loop_; }
  int fd() const { return fd_; }
  uint32_t getEvents() const { return events_; }
  uint32_t getRevents() const { return revents_; }
  int getState() const { return state_; }

  void setFd(int fd) { fd_ = fd; }
  void setEvents(uint32_t events) { events_ = events; }
  void setRevents(uint32_t revents) { revents_ = revents; }
  void setState(int state);

  void setReadCallBack(CallBack &read) { readCb_ = read; }
  void setWriteCallBack(CallBack &write) { writeCb_ = write; }
  void setErrorCallBack(CallBack &error) { errorCb_ = error; }
  void SetReadCallBack(CallBack &&read) { readCb_ = std::move(read); }
  void SetWriteCallBack(CallBack &&write) { writeCb_ = std::move(write); }
  void SetErrorCallBack(CallBack &&error) { errorCb_ = std::move(error); }

  void SetRead(bool label);
  void SetWrite(bool label);
  void SetError(bool label);

  void handleRead();
  void handleWrite();
  void handleError();
  void handleEvent();

  // 表示一个channel的状态：新建的、添加到循环中的、删除的
  static const int kNew;
  static const int kAdded;
  static const int kDeleted;

  // channel事件
  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;
  static const int kErrorEvent;

 private:
  EventLoop *loop_;
  int fd_;
  uint32_t events_;
  uint32_t revents_; // recieve events from poller.
  int state_;

  // 回调函数
  CallBack readCb_;
  CallBack writeCb_;
  CallBack errorCb_;
};

} // namespace web

#endif // _CHANNEL_H