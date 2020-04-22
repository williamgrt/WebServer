#include "Channel.h"
#include <sys/epoll.h>

namespace gnet {
Channel::Channel(EventLoop *ev, int fd) : ev_(ev), fd_(fd), events_(0) {}

Channel::~Channel() {
  events_ = 0;
  fd_ = -1;
}

void Channel::SetRead() { events_ |= EPOLLIN; }

} // namespace gnet
