#include "Poller.h"
#include "Utils.h"

namespace gnet {
Poller::Poller(EventLoop *ev) : ev_(ev) { epollFd_ = ::epoll_create(1024); }

Poller::~Poller() {
  if (epollFd_ != -1) {
    ::close(epollFd_);
  }
}

int Poller::AddChannel(types::ChannelPtr channel) {
  epoll_event dummy;
  bzero(&dummy, sizeof(dummy));

  dummy.events =
}

} // namespace gnet
