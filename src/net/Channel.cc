#include "Channel.h"

namespace gnet {
Channel::Channel(EventLoop *ev, int fd, short events)
    : ev_(ev), fd_(fd), events_(events) {
  poller_ = ev_->GetPoller();
}

Channel::~Channel() {
  events_ = 0;
  fd_ = -1;
}

} // namespace gnet
