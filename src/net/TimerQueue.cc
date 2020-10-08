#include "TimerQueue.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Defs.h"
#include <sys/timerfd.h>
#include <algorithm>

using namespace web;
using namespace std;

namespace web {
namespace timer {
// 封装操作timerfd的函数

/**********
 * @brief 创建新的timerfd
 * @return 不为0的timerfd描述符
 */
int createTimerFd() {
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  if (timerfd < 0) {
    abort();
  }
  return timerfd;
}

/**********
 * @brief 读取timerfd的内容，
 * @param timerfd 指定的timerfd
 */
void readTimerFd(int timerfd) {
  uint64_t numExp;

  std::size_t n = ::read(timerfd, &numExp, sizeof(uint64_t));
  if (n != sizeof(numExp)) {
    // TODO: add log codes
  }
}

/********
 * @brief 计算指定时间距离当前时间的差值
 * @param when
 * @return
 */
timespec howMuchTimeFromNow(Timer::TimeType when) {
  int64_t microseconds = Timer::now() - when;
  // 精度调整
  if (microseconds <= 100) {
    microseconds = 100;
  }

  timespec ts{};
  ts.tv_sec = static_cast<time_t>(microseconds / Timer::kMsPerSec);
  ts.tv_nsec = static_cast<long>((microseconds % Timer::kMsPerSec) * 1000);
  return ts;
}

/*
 * 重置定时器的间隔时间
 */
void resetTimerFd(int timerfd, Timer::TimeType when) {
  itimerspec newVal{};

  newVal.it_value = howMuchTimeFromNow(when);
  int r = ::timerfd_settime(timerfd, 0, &newVal, nullptr);
  if (r < 0) {
    // TODO: Add log codes
  }
}

}
}

TimerQueue::TimerQueue(EventLoop *loop)
    : loop_(loop),
      timerfd_(timer::createTimerFd()),
      timerfdChannel_(new Channel(loop_, timerfd_)),
      mutex_() {
  timerfdChannel_->setReadCallback(std::bind(&TimerQueue::handleRead, this));
  timerfdChannel_->enableRead();
}

TimerQueue::~TimerQueue() {
  ::close(timerfd_);
}

/*
 * 往定时器队列中添加新的定时器
 * 使用互斥锁保证线程安全
 */
TimerId TimerQueue::addTimer(Timer::TimeType now, Timer::TimerCallBack cb, Timer::TimeType interval) {
  // 可能会有多个线程同时添加计时器
  // 这样的话会有线程不安全的风险
  // muduo 采用的方式是添加到 I/O 线程中解决
  // 本项目使用的方式是添加互斥锁
  // 这两种方式到底有什么差别？（性能？/使用效率？）
  lock_guard<mutex> guard(mutex_);
  TimerPtr newTimer = make_shared<Timer>(now, cb, interval);
  // 插入新的定时器
  bool result = addTimer(newTimer);
  // 如果新插入的定时器是最小的定时器，那么更新 timerfd 的过期时间
  if (result) {
    web::timer::resetTimerFd(timerfd_, now);
  }
  TimerId timerId(newTimer);
  return timerId;
}

/*
 * 往定时器队列中插入定时器
 * 判断是否需要更新计时器的触发时间
 */
bool TimerQueue::addTimer(const TimerPtr &timer) {
  assert(timer != nullptr);

  bool isFirstTimer = false;
  // 如果定时器队列为空或者第一个定时器的过期时间小于新插入定时器的更新时间
  // 需要重新设置定时器的时间
  if (timers_.empty() || timers_.begin()->first > timer->getExpire()) {
    isFirstTimer = true;
  }
  // 往定时器列表中插入
  auto result = timers_.insert(std::make_pair(timer->getExpire(), timer));
  assert(result.second);

  return isFirstTimer;
}

/*
 * 删除特定的定时器
 */
void TimerQueue::cancel(TimerId timerId) {
  assert(timerId.timer_.lock() != nullptr);
  lock_guard<mutex> guard(mutex_);

  TimerPtr delTimer = timerId.timer_.lock();
  Entry entry = make_pair(delTimer->getExpire(), delTimer);
  auto it = timers_.find(entry);
  if (it != timers_.end()) {
    timers_.erase(it);
  }
}

/*
 * 获取所有到时的定时器
 */
vector<TimerQueue::Entry> TimerQueue::getExpiredTimer(Timer::TimeType now) {
  std::vector<TimerQueue::Entry> expired;

  // 返回第一个未到期的计时器
  Timer::TimeType curr = Timer::now();
  Entry entry = make_pair(curr, std::shared_ptr<Timer>());
  {
    lock_guard<mutex> guard(mutex_);
    auto it = std::lower_bound(timers_.begin(), timers_.end(), entry);
    assert(it == timers_.end() || now < it->first);
    // 获取所有的过期定时器
    std::copy(timers_.begin(), it, std::back_inserter(expired));
    // 删除所有的到期定时器
    timers_.erase(timers_.begin(), it);
  }
  return expired;
}

/*
 * 当 timerfd 超时触发时调用
 */
void TimerQueue::handleRead() {
  // 通过 EventLoop 调用
  loop_->assertInLoopThread();
  // 确保不会再次触发
  web::timer::readTimerFd(timerfd_);

  Timer::TimeType now = Timer::now();
  std::vector<Entry> expired = getExpiredTimer(now);
  for (auto &entry: expired) {
    entry.second->run();
  }
  reset(expired);
}

void TimerQueue::reset(vector<Entry> &expired) {
  // 通过互斥锁保护定时器队列
  lock_guard<mutex> guard(mutex_);
  // 重置定时器
  for (auto it: expired) {
    // 重置循环定时器的时间
    if (it.second->isRepeated()) {
      it.second->restart();
      addTimer(it.second);
    }
  }

  // 设置新的定时器时间
  if (!timers_.empty()) {
    TimerPtr timer = timers_.begin()->second;
    if (timer->isValid()) {
      web::timer::resetTimerFd(timerfd_, timer->getExpire());
    }
  }
}