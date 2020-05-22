#include "EPoller.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Utils.h"
#include <cstring>

using namespace web;

EPoller::EPoller() {
  epollfd_ = ::epoll_create1(EPOLL_CLOEXEC);
  assert(epollfd_ > 0);
}

EPoller::~EPoller() {
  if (epollfd_ != -1) {
    // epoll 没有被关闭
    ::close(epollfd_);
  }
}

void EPoller::addChannel(Channel *channel) {
  assert(channel != nullptr);
  assert(channel->fd() != -1);
  assert(channel->getState() != Channel::kAdded);

  int fd = channel->fd();
  epoll_event event;
  bzero(&event, sizeof(event));
  event.events = channel->getEvents() | EPOLLET; // 采用边缘触发模式
  event.data.ptr = channel;

  int r = ::epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event);
  if (r == -1) {
    error(1, errno, "epoll addTimer error.");
  }

  fd2Channel_[fd] = channel;
  channel->setState(Channel::kAdded);
}

void EPoller::modifyChannel(Channel *channel) {
  assert(channel != nullptr);
  assert(channel->fd() != -1);

  int fd = channel->fd();
  assert(fd2Channel_.count(fd) != 0); // 必须是列表里面出现过的channel
  assert(fd2Channel_[fd] == channel);

  epoll_event event;
  bzero(&event, sizeof(event));
  event.events = channel->getEvents();
  event.data.ptr = channel;

  int r = ::epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &event);
  if (r == -1) {
    perror("epoll modify error.");
  }
}

void EPoller::deleteChannel(Channel *channel) {
  assert(channel != nullptr);
  int fd = channel->fd();
  assert(fd2Channel_.count(fd) != 0 && fd2Channel_[fd] == channel);

  epoll_event event;
  bzero(&event, sizeof(event));
  event.events = channel->getEvents();
  event.data.ptr = channel;

  fd2Channel_.erase(fd); // 从事件集合中删除
  // 如果在监听的事件集中，删除这个事件
  if (channel->getState() == Channel::kAdded) {
    int r = ::epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &event);
    if (r == -1) {
      perror("epoll delete error.");
    }
  }
  channel->setState(Channel::kDeleted);
}

int EPoller::poll(int maxEvent, int waitMs,
                  std::vector<Channel *> &activeChannels) {
  // 确保epoll已经初始化
  assert(epollfd_ != -1);
  assert(maxEvent > 0);
  assert(waitMs >= -1);

  // 将events_的容量修改到最大的事件数
  events_.resize(maxEvent);
  // 开始监听
  int nReady = ::epoll_wait(epollfd_, &*events_.begin(), maxEvent, waitMs);
  if (nReady < 0) {
    perror("epoll wait error.");
  } else if (nReady == 0) {
    // TODO: handle empty result
  } else {
    for (int i = 0; i < nReady; ++i) {
      Channel *currChannel = static_cast<Channel *>(events_[i].data.ptr);
      short revents = events_[i].events;
      currChannel->setRevents(revents);
      activeChannels.push_back(currChannel);
    }
  }

  return nReady;
}
