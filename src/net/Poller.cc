#include "Poller.h"
#include "Utils.h"

namespace gnet {
Poller::Poller(EventLoop *ev) : ev_(ev) { epollFd_ = ::epoll_create(1024); }

Poller::~Poller() {
  if (epollFd_ != -1) {
    ::close(epollFd_);
  }
}

bool Poller::AddChannel(types::ChannelPtr channel) {
  epoll_event dummy;
  bzero(&dummy, sizeof(dummy));

  dummy.events = channel->GetEvents() | EPOLLET; // 采用边缘触发模式
  dummy.data.fd = channel->GetFd();

  int r = epoll_ctl(epollFd_, EPOLL_CTL_ADD, channel->GetFd(), &dummy);
  if (r == -1) {
    error(1, errno, "epoll add error.");
    return false;
  }

  fd2Event[channel->GetFd()] = channel;
  return true;
}

bool Poller::ModifyChannel(types::ChannelPtr channel) {}

} // namespace gnet
