// 测试EventLoop程序功能
// 参考陈硕muduo的测试代码

#include "../Channel.h"
#include "../EventLoop.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <sys/timerfd.h>

using namespace web;
using namespace std;

EventLoop *currLoop;

void timeout() {
  cout << "Time Out\n";
  currLoop->quit();
}

int main() {
  EventLoop loop;
  currLoop = &loop;

  int timer = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  struct itimerspec howlong;
  memset(&howlong, 0, sizeof howlong);
  howlong.it_value.tv_sec = 5;
  ::timerfd_settime(timer, 0, &howlong, NULL);

  auto channel = make_shared<Channel>(&loop, timer);
  channel->enableRead();
  channel->setReadCallback(timeout);
  cout << "start looping\n";

  loop.loop();
}