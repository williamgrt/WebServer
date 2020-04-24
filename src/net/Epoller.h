#ifndef _EPOLLER_H
#define _EPOLLER_H

#include <sys/epoll.h>
#include <unordered_map>
#include <vector>

namespace web {
class EventLoop;
class Channel;

class Epoller {
private:
  int epollFd_;
  std::vector<epoll_event> events_;
  std::unordered_map<int, Channel *> fd2Channel_;

public:
  Epoller();
  ~Epoller();

  void AddChannel(Channel *channel);
  void ModifyChannel(Channel *channel);
  void DeleteChannel(Channel *channel);

  int GetFd() { return epollFd_; }

  // 打开epoll并进行监听
  int Poll(int maxEvent, int waitMs, std::vector<Channel *> &activeChannels);
};

} // namespace web

#endif // _EPOLLER_H