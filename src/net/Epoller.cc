#include "Epoller.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Utils.h"
#include <cstring>

using namespace web;

Epoller::Epoller() {
  epollFd_ = ::epoll_create1(EPOLL_CLOEXEC);
  assert(epollFd_ > 0);
}

Epoller::~Epoller() {
  if (epollFd_ != -1) {
    // epoll 没有被关闭
    ::close(epollFd_);
  }
}

void Epoller::AddChannel(Channel *channel) {
  assert(channel != nullptr);
  assert(channel->getFd() != -1);
  assert(channel->GetState() != Channel::kAdded);

  int fd = channel->getFd();
  epoll_event event;
  bzero(&event, sizeof(event));
  event.events = channel->getEvents() | EPOLLET; // 采用边缘触发模式
  event.data.ptr = channel;

  int r = ::epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event);
  if (r == -1) {
    error(1, errno, "epoll add error.");
  }

  fd2Channel_[fd] = channel;
  channel->SetState(Channel::kAdded);
}

void Epoller::ModifyChannel(Channel *channel) {
  assert(channel != nullptr);
  assert(channel->getFd() != -1);

  int fd = channel->getFd();
  assert(fd2Channel_.count(fd) != 0); // 必须是列表里面出现过的channel
  assert(fd2Channel_[fd] == channel);

  epoll_event event;
  bzero(&event, sizeof(event));
  event.events = channel->getEvents();
  event.data.ptr = channel;

  int r = ::epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event);
  if (r == -1) {
    perror("epoll modify error.");
  }
}

void Epoller::DeleteChannel(Channel *channel) {
  assert(channel != nullptr);
  int fd = channel->getFd();
  assert(fd2Channel_.count(fd) != 0 && fd2Channel_[fd] == channel);

  epoll_event event;
  bzero(&event, sizeof(event));
  event.events = channel->getEvents();
  event.data.ptr = channel;

  fd2Channel_.erase(fd); // 从事件集合中删除
  // 如果在监听的事件集中，删除这个事件
  if (channel->GetState() == Channel::kAdded) {
    int r = ::epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event);
    if (r == -1) {
      perror("epoll delete error.");
    }
  }
  channel->SetState(Channel::kDeleted);
}

int Epoller::Poll(int maxEvent, int waitMs,
                  std::vector<Channel *> &activeChannels) {
  // 确保epoll已经初始化
  assert(epollFd_ != -1);
  assert(maxEvent > 0);
  assert(waitMs >= -1);

  // 将events_的容量修改到最大的事件数
  events_.resize(maxEvent);
  // 开始监听
  int nReady = ::epoll_wait(epollFd_, &*events_.begin(), maxEvent, waitMs);
  if (nReady < 0) {
    perror("epoll wait error.");
  } else if (nReady == 0) {
    // TODO: handle empty result
  } else {
    for (int i = 0; i < nReady; ++i) {
      Channel *currChannel = static_cast<Channel *>(events_[i].data.ptr);
      short revents = events_[i].events;
      currChannel->SetRevents(revents);
      activeChannels.push_back(currChannel);
    }
  }

  return nReady;
}
