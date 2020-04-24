#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <functional>
#include <memory>
#include <poll.h>

namespace web {
using CallBack = std::function<void()>;

class EventLoop;
class Epoller;

class Channel {
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

public:
  Channel(EventLoop *ev, int fd);
  Channel(EventLoop *ev);
  ~Channel();

  EventLoop *GetLoop() const { return loop_; }
  int GetFd() const { return fd_; }
  uint32_t GetEvents() const { return events_; }
  uint32_t GetRevents() const { return revents_; }
  int GetState() const { return state_; }

  void SetFd(int fd);
  void SetEvents(uint32_t events) { events_ = events; }
  void SetRevents(uint32_t revents) { revents_ = revents; }
  void SetState(int state);

  void SetReadCallBack(CallBack &read) { readCb_ = read; }
  void SetWriteCallBack(CallBack &write) { writeCb_ = write; }
  void SetErrorCallBack(CallBack &error) { errorCb_ = error; }
  void SetReadCallBack(CallBack &&read) { readCb_ = std::move(read); }
  void SetWriteCallBack(CallBack &&write) { writeCb_ = std::move(write); }
  void SetErrorCallBack(CallBack &&error) { errorCb_ = std::move(error); }

  void SetRead(bool label);
  void SetWrite(bool label);
  void SetError(bool label);

  void HandleRead();
  void HandleWrite();
  void HandleError();
  void HandleEvent();

  // 表示一个channel的状态：新建的、添加到循环中的、删除的
  static const int kNew = -1;
  static const int kAdded = 1;
  static const int kDeleted = 2;

  // channel事件
  const int kNoneEvent = 0;
  const int kReadEvent = POLLIN | POLLPRI;
  const int kWriteEvent = POLLOUT;
  const int kErrorEvent = POLLERR;
};

} // namespace web

#endif // _CHANNEL_H