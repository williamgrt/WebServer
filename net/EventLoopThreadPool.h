#ifndef WEBSERVER_SRC_NET_EVENTLOOPTHREADPOOL_H
#define WEBSERVER_SRC_NET_EVENTLOOPTHREADPOOL_H

#include <memory>
#include <vector>
#include <utils/noncopyable.h>
namespace gweb {
class EventLoopThread;
class EventLoop;

class EventLoopThreadPool : noncopyable {
public:
  EventLoopThreadPool(EventLoop *baseLoop, int threadNum);
  explicit EventLoopThreadPool(EventLoop *baseLoop);
  ~EventLoopThreadPool();

  void setThreadNum(int threadNum);
  EventLoop *getNextLoop();

  void start();

private:
  EventLoop *baseLoop_;
  int threadNum_;
  bool started_;
  std::vector<std::unique_ptr<EventLoopThread>> pool_;
  std::vector<EventLoop *> loops_;
  int nextLoopIndex_; // 指向下一个被分配的 EventLoop
};

}

#endif //WEBSERVER_SRC_NET_EVENTLOOPTHREADPOOL_H
