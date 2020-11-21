#ifndef WEBSERVER_SRC_NET_EPOLLER_H
#define WEBSERVER_SRC_NET_EPOLLER_H

#include <sys/epoll.h>
#include <unordered_map>
#include <vector>

#include <gnet/utils/noncopyable.h>
#include <gnet/net/Timer.h>

namespace gweb {
// 由于没有包含相应的头文件，必须提前声明
class EventLoop;
class Channel;

class Epoll : noncopyable {
 private:
  int epollfd_;
  std::vector<epoll_event> events_;
  std::unordered_map<int, Channel *> fd2Channel_;

 public:
  Epoll();
  ~Epoll();

  void add(Channel *channel);
  void modify(Channel *channel);
  void remove(Channel *channel);

  int fd() const { return epollfd_; }

  // 打开epoll并进行监听
  Timer::TimeType poll(int maxEvent, int waitMs, std::vector<Channel *> &activeChannels);
};

} // namespace web

#endif // _EPOLLER_H