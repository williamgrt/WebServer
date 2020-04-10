#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

namespace gnet {
class EventLoop {
private:
  /* data */
  types::PollerPtr poller_;

public:
  EventLoop(/* args */);
  ~EventLoop();

  types::PollerPtr GetPoller() { return poller_; }
};

EventLoop::EventLoop(/* args */) {}

EventLoop::~EventLoop() {}

} // namespace gnet

#endif // _EVENTLOOP_H