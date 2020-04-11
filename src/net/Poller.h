#ifndef _POLLER_H
#define _POLLER_H

#include "EventLoop.h"
#include <sys/epoll.h>
#include <unordered_map>
#include <vector>

namespace gnet {
class Poller {
private:
  EventLoop *ev_;
  int epollFd_;
  std::vector<epoll_event> liveEvents_;
  std::unordered_map<int, types::ChannelPtr> fd2Event;

public:
  Poller(EventLoop *ev);
  ~Poller();

  bool AddChannel(types::ChannelPtr channel);
  bool ModifyChannel(types::ChannelPtr channel);
  bool DeleteChannel(types::ChannelPtr channel);

  EventLoop *GetEv() { return ev_; }
  int GetFd() { return epollFd_; }

  void LoopOnce(int waitMs);
};

} // namespace gnet

#endif // _POLLER_H