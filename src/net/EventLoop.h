#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

namespace gnet {
// 所有eventloop的基类
class EventLoopBase {
public:
  virtual EventLoopBase *GetNextLoop() = 0;
};

// 单线程版本的EventLoop
class EventLoop : public EventLoopBase {
private:
  types::PollerPtr poller_;
  bool started_;

public:
  EventLoop(/* args */);
  ~EventLoop();

  types::PollerPtr GetPoller() { return poller_; }

  EventLoopBase *GetNextLoop() override { return this; }
};

EventLoop::EventLoop(/* args */) {}

EventLoop::~EventLoop() {}

} // namespace gnet

#endif // _EVENTLOOP_H