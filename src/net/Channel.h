#ifndef _CHANNEL_H
#define _CHANNEL_H

#include "EventLoop.h"
#include "Poller.h"
#include <functional>
#include <memory>

namespace gnet {
using CallBack = std::function<void()>;

class Channel {
private:
  EventLoop *ev_;
  types::PollerPtr poller_;
  int fd_;
  short events_;

  CallBack readCb_, writeCb_;

public:
  Channel(EventLoop *ev, int fd, short events);
  ~Channel();

  EventLoop *GetEv() { return ev_; }
  int GetFd() { return fd_; }
  short GetEvents() { return events_; }
  void Close();

  void OnRead(CallBack &read) { readCb_ = read; }
  void OnWrite(CallBack &write) { writeCb_ = write; }
  void OnRead(CallBack &&read) { readCb_ = std::move(read); }
  void OnWrite(CallBack &&write) { writeCb_ = std::move(write); }

  void SetRead(short status);
  void SetWrite(short status);
  void SetReadWrite(short status);
  bool EnabledRead();
  bool EnabledWrite();

  void HandleRead() { readCb_(); }
  void HandleWrite() { writeCb_(); }
};

} // namespace gnet

#endif // _CHANNEL_H