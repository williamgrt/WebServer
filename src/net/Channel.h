#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <functional>
#include <memory>

namespace gnet {
using ReadCallBack = std::function<void()>;
using WriteCallBack = std::function<void()>;

class EventLoop;
class Poller;

class Channel {
private:
  EventLoop *ev_;
  int fd_;
  short events_;
  short revents_; // recieve events from poller.
  int state_;

  // 回调函数
  ReadCallBack readCb_;
  WriteCallBack writeCb_;

public:
  Channel(EventLoop *ev, int fd);
  ~Channel();

  EventLoop *GetLoop() { return ev_; }
  int GetFd() { return fd_; }
  short GetEvents() { return events_; }
  short GetRevents() { return revents_; }
  short SetRevents(short revents) { revents_ = revents; }

  void SetReadCallBack(ReadCallBack &read) { readCb_ = read; }
  void SetWriteCallBack(WriteCallBack &write) { writeCb_ = write; }
  void SetReadCallBack(ReadCallBack &&read) { readCb_ = std::move(read); }
  void SetWriteBallBack(WriteCallBack &&write) { writeCb_ = std::move(write); }

  void SetRead();
  void SetWrite();
  void SetReadWrite();
  bool ReadEnabled();
  bool WriteEnabled();

  void HandleRead() { readCb_(); }
  void HandleWrite() { writeCb_(); }
};

} // namespace gnet

#endif // _CHANNEL_H