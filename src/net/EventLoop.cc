#include "EventLoop.h"
#include "Channel.h"
#include "EPoller.h"
#include "Utils.h"
#include <algorithm>

using namespace web;
using namespace std;

thread_local web::EventLoop *loopInThisThread = nullptr; // 当前线程的eventloop
const int MAX_EVENT_NUM = 1024;

EventLoop::EventLoop()
    : looping_(false), quit_(false), eventCapacity_(MAX_EVENT_NUM), poller_(new EPoller()) {
  assert(loopInThisThread == nullptr);
  loopInThisThread = this;
}

EventLoop::~EventLoop() {
  looping_ = false;
  loopInThisThread = nullptr;
}

void EventLoop::addChannel(Channel *channel) {
  assert(channel != nullptr);
  assert(channel->getLoop() == this);

  poller_->addChannel(channel);
}

void EventLoop::assertInLoopThread() {
  assert(loopInThisThread == this);
}

void EventLoop::modifyChannel(Channel *channel) {
  assert(channel != nullptr);
  assert(channel->getState() == Channel::kAdded);
  assert(channel->getLoop() == this);

  poller_->modifyChannel(channel);
}

void EventLoop::deleteChannel(Channel *channel) {
  poller_->deleteChannel(channel);
}

void EventLoop::loop() {
  assert(!looping_);
  assert(!quit_);
  looping_ = true;
  quit_ = false;

  std::vector<Channel *> activeChannels;
  while (!quit_) {
    activeChannels.clear();
    poller_->poll(eventCapacity_, -1, activeChannels);
    for (auto currChannel : activeChannels) {
      currChannel->handleEvent();
    }
  }

  looping_ = false;
}

void EventLoop::close() {
  quit_ = true;
}

bool EventLoop::isInLoopThread() {
  return loopInThisThread == this;
}

