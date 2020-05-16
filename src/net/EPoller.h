#ifndef WEBSERVER_SRC_NET_EPOLLER_H
#define WEBSERVER_SRC_NET_EPOLLER_H

#include <sys/epoll.h>
#include <unordered_map>
#include <vector>
#include "../base/noncopyable.h"

namespace web {
// 由于没有包含相应的头文件，必须提前声明
class EventLoop;
class Channel;

class EPoller : noncopyable {
 private:
  int epollfd_;
  std::vector<epoll_event> events_;
  std::unordered_map<int, Channel *> fd2Channel_;

 public:
  EPoller();
  ~EPoller();

  void addChannel(Channel *channel);
  void modifyChannel(Channel *channel);
  void deleteChannel(Channel *channel);

  int fd() const { return epollfd_; }

  // 打开epoll并进行监听
  int poll(int maxEvent, int waitMs, std::vector<Channel *> &activeChannels);
};

} // namespace web

#endif // _EPOLLER_H