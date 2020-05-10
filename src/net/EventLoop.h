#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <memory>
#include <thread>
#include <vector>

namespace web {
class Channel;
class EPoller;

// 单线程版本的EventLoop
class EventLoop {
private:
  bool looping_;
  bool quit_;
  int eventCapacity_;
  std::unique_ptr<EPoller> poller_;

public:
  EventLoop();
  ~EventLoop();

  void addChannel(Channel *channel);
  void modifyChannel(Channel *channel);
  void deleteChannel(Channel *channel);

  // EventLoop运行函数
  void loop();

  void assertInLoopThread();
};

} // namespace web

#endif // _EVENTLOOP_H