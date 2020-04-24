#include "EventLoop.h"
#include "Channel.h"
#include "Epoller.h"
#include "Utils.h"
#include <algorithm>

using namespace web;

__thread web::EventLoop *_loopInThisThread = nullptr; // 当前线程的eventloop

EventLoop::EventLoop() : poller_(new Epoller()), looping_(false), quit_(false) {
  assert(_loopInThisThread == nullptr);
  _loopInThisThread = this;
}

EventLoop::~EventLoop() {
  looping_ = false;
  _loopInThisThread = nullptr;
}

void EventLoop::AddChannel(Channel *channel) {
  assert(channel != nullptr);
  assert(channel->GetLoop() == this);

  poller_->AddChannel(channel);
}

void EventLoop::ModifyChannel(Channel *channel) {
  assert(channel != nullptr);
  assert(channel->GetState() == Channel::kAdded);
  assert(channel->GetLoop() == this);

  poller_->ModifyChannel(channel);
}

void EventLoop::DeleteChannel(Channel *channel) {
  poller_->DeleteChannel(channel);
}

void EventLoop::Loop() {
  assert(!looping_);
  assert(!quit_);

  // 开始循环
  looping_ = true;
  std::vector<Channel *> activeChannels;
  while (!quit_) {
    activeChannels.clear();
    int nReady = poller_->Poll(eventCapacity_, -1, activeChannels);
    for (auto currChannel : activeChannels) {
      currChannel->HandleEvent();
    }
  }
}