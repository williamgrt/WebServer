#include "Channel.h"

namespace gnet {
Channel::Channel(EventLoop *ev, int fd, short events)
    : ev_(ev), fd_(fd), events_(events) {
  poller_ = ev_->GetPoller();
}
} // namespace gnet
