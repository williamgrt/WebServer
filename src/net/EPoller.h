#ifndef _EPOLLER_H
#define _EPOLLER_H

#include <sys/epoll.h>
#include <unordered_map>
#include <vector>

namespace web {
// 由于没有包含相应的头文件，必须提前声明
class EventLoop;
class Channel;

class EPoller {
 private:
  int epollFd_;
  std::vector<epoll_event> events_;
  std::unordered_map<int, Channel *> fd2Channel_;

 public:
  EPoller();
  ~EPoller();

  void addChannel(Channel *channel);
  void modifyChannel(Channel *channel);
  void deleteChannel(Channel *channel);

  int fd() {
    return epollFd_;
  }

  // 打开epoll并进行监听
  int poll(int maxEvent, int waitMs, std::vector<Channel *> &activeChannels);
};

} // namespace web

#endif // _EPOLLER_H