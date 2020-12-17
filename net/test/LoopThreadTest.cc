//
// Created by 郭睿韬 on 2020/6/20.
//

#include <unistd.h>
#include <iostream>
#include "net/EventLoopThread.h"
#include "net/EventLoop.h"

using namespace gweb;
using namespace std;

void runThread() {
  cout << "pid = " << getpid() << ", tid = " << this_thread::get_id() << "\n";
}

int main() {
  cout << "pid = " << getpid() << ", tid = " << this_thread::get_id() << "\n";
  EventLoopThread thread;

  EventLoop *loop = thread.start();
  loop->runInLoop(runThread);
  sleep(1);
  loop->runAfter(2, runThread);
  sleep(3);
  loop->quit();
  return 0;
}