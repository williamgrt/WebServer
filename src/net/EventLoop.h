#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <memory>
#include <thread>
#include <vector>

namespace web {
class Channel;
class Epoller;

// 单线程版本的EventLoop
class EventLoop {
private:
  bool looping_;
  bool quit_;
  int eventCapacity_;
  std::unique_ptr<Epoller> poller_;

public:
  EventLoop();
  ~EventLoop();

  void AddChannel(Channel *channel);
  void ModifyChannel(Channel *channel);
  void DeleteChannel(Channel *channel);
  bool HasChannel(Channel *channel);

  void Loop();

  void AssertInLoopThread();
  bool LoopInThread();
};

} // namespace web

#endif // _EVENTLOOP_H