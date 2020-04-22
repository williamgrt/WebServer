#ifndef _POLLER_H
#define _POLLER_H

#include <sys/epoll.h>
#include <unordered_map>
#include <vector>

namespace gnet {
class EventLoop;
class Channel;

class Poller {
private:
  EventLoop *ev_;
  int epollFd_;
  std::vector<epoll_event> events_;
  std::unordered_map<int, Channel *> fd2Event_;

public:
  Poller(EventLoop *ev);
  ~Poller();

  void AddChannel(Channel *channel);
  void ModifyChannel(Channel *channel);
  void DeleteChannel(Channel *channel);

  EventLoop *GetLoop() { return ev_; }
  int GetFd() { return epollFd_; }

  // 打开epoll并进行监听
  int Poll(int maxEvent, int waitMs, std::vector<Channel *> &activeChannels);
};

} // namespace gnet

#endif // _POLLER_H