#include "EventLoop.h"
#include "Channel.h"
#include "EPoller.h"
#include "Utils.h"
#include <algorithm>
#include <sys/eventfd.h>

using namespace web;
using namespace std;

thread_local web::EventLoop *loopInThisThread = nullptr; // 当前线程的 EventLoop, 保证 one loop per thread
const int MAX_EVENT_NUM = 1024;

/*****
 * @brief
 * @return
 */
int createEventFd() {
  int evfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evfd < 0) {
    // TODO: Add code for log output
    abort();
  }
  return evfd;
}

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      eventCapacity_(MAX_EVENT_NUM),
      poller_(new EPoller()),
      ownerId_(this_thread::get_id()),
      wakeupFd_(createEventFd()),
      wakeupChannel_(new Channel(this, wakeupFd_)),
      callingPendingFunctors_(false),
      timerQueue_(make_unique<TimerQueue>(this)) {
  assert(loopInThisThread == nullptr);
  loopInThisThread = this;
  wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleWakeup, this));
  wakeupChannel_->enableRead();
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

void EventLoop::modifyChannel(Channel *channel) {
  assert(channel != nullptr);
  assert(channel->getState() == Channel::kAdded);
  assert(channel->getLoop() == this);
  poller_->modifyChannel(channel);
}

void EventLoop::deleteChannel(Channel *channel) {
  assert(channel != nullptr);
  assert(channel->getState() == Channel::kAdded);
  assert(channel->getLoop() == this);
  poller_->removeChannel(channel);
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
    doPendingFunctors();
  }

  looping_ = false;
}

void EventLoop::quit() {
  quit_ = true;
  // 需要通知循环已结束
  if (!isInLoopThread()) {
    wakeup();
  }
}

bool EventLoop::isInLoopThread() const {
  // cout << "ownerId_: " << ownerId_ << " this thread: " << this_thread::get_id() << "\n";
  return ownerId_ == this_thread::get_id();
}

void EventLoop::assertInLoopThread() const {
  assert(isInLoopThread());
}

void EventLoop::runInLoop(const Functor &cb) {
  if (isInLoopThread()) {
    // 在本线程中调用
    cb();
  } else {
    // 在其他线程调用
    // 需要把任务添加到 EventLoop 的任务队列中，然后
    queueInLoop(cb);
  }
}

void EventLoop::queueInLoop(const Functor &cb) {
  {
    // 保证线程安全
    // 有可能会有多个工作线程调用该函数
    // 必须同时只有一个线程添加任务
    lock_guard<mutex> lock(mutex_);
    pendingFunctors_.push_back(cb);
  }

  // 什么时候需要唤醒 I/O 线程？
  // 1. 在非 I/O 线程中调用该函数
  // 2. 在 I/O 线程线程调用函数，并且 EventLoop 正在执行 doPendingFunctors
  //    此时，为了保证插入的函数能够快速执行，需要唤醒 eventloop，在 doPendingFunctors 结束后，能够立即执行
  if (!isInLoopThread() || callingPendingFunctors_) {
    wakeup();
  }
}

void EventLoop::doPendingFunctors() {
  callingPendingFunctors_ = true;
  std::vector<Functor> functorList;
  // 仅在交换 functor 期间加锁
  // 缩短临界区，防止 queueInLoop 等待过长时间
  {
    lock_guard<mutex> guard(mutex_);
    functorList.swap(pendingFunctors_);
  }
  for (const auto &functor: functorList) {
    functor();
  }
  callingPendingFunctors_ = false;
}

void EventLoop::wakeup() {
  // 写入一个字节的数据
  uint64_t one = 1;
  ssize_t r = ::write(wakeupFd_, &one, sizeof(one));
  if (r != sizeof(one)) {
    // TODO: Add code for log output
    perror("Write eventfd error");
  }
}

void EventLoop::handleWakeup() {
  uint64_t one = 1;
  ssize_t r = ::read(wakeupFd_, &one, sizeof(one));
  if (r != sizeof(one)) {
    // TODO: Add code for log output
    perror("Read eventfd error");
  }
}

TimerId EventLoop::runAfter(double delay, const Functor &cb) {
  Timer::TimeType when = Timer::now() + static_cast<Timer::TimeType>(delay * Timer::kMsPerSec);
  return timerQueue_->addTimer(when, cb, 0);
}

TimerId EventLoop::runEvery(double interval, const Functor &cb) {
  assert(interval > 0);
  Timer::TimeType when = Timer::now() + static_cast<Timer::TimeType>(interval * Timer::kMsPerSec);
  return timerQueue_->addTimer(when, cb, static_cast<Timer::TimeType>(interval * Timer::kMsPerSec));
}

void EventLoop::cancel(TimerId timerId) {
  timerQueue_->cancel(timerId);
}
