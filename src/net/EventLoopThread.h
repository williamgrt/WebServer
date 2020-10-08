#ifndef WEBSERVER_SRC_NET_EVENTLOOPTHREAD_H
#define WEBSERVER_SRC_NET_EVENTLOOPTHREAD_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include "Defs.h"

namespace web {
class EventLoop;

class EventLoopThread: noncopyable {
public:
  EventLoopThread();
  ~EventLoopThread();

  EventLoop * start();

private:
  void threadFunc();

  EventLoop *loop_;
  std::thread thread_;
  std::mutex mutex_;
  std::condition_variable cond_;
};
}

#endif //WEBSERVER_SRC_NET_EVENTLOOPTHREAD_H
