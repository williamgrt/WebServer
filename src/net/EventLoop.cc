#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include <algorithm>

__thread gnet::EventLoop *_loopInThisThread = nullptr;

namespace gnet {

EventLoop::EventLoop()
    : poller_(new Poller(this)), loop_(false), quit_(false), maxEvents_(1024) {}

EventLoop::~EventLoop() {}

void EventLoop::AddChannel(const ChannelPtr &channel) {
  channels_.push_back(channel);
  poller_->AddChannel(channel.get());
}

void EventLoop::ModifyChannel(const ChannelPtr &channel) {
  poller_->ModifyChannel(channel.get());
}

void EventLoop::DeleteChannel(const ChannelPtr &channel) {
  poller_->DeleteChannel(channel.get());
  auto ptr = channels_.begin();
  for (; *ptr != channel && ptr != channels_.end(); ++ptr) {
  }
  channels_.erase(ptr);
}

void EventLoop::Loop() {
  std::vector<Channel *> activeChannels;

  while (!quit_) {
    activeChannels.clear();
    int nReady = poller_->Poll(maxEvents_, -1, activeChannels);
    for (auto currChannel : activeChannels) {
      int revents = currChannel->GetRevents();
      if (revents & EPOLLIN) {
        currChannel->HandleRead();
      }
      if (revents & EPOLLOUT) {
        currChannel->HandleWrite();
      }
    }
  }
}

} // namespace gnet
