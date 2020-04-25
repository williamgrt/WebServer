#ifndef _THREAD_H
#define _THREAD_H

#include "noncopyable.h"
#include <atomic>
#include <functional>
#include <pthread.h>
#include <string>

namespace web {
using Functor = std::function<void()>;

class Thread : public noncopyable {
private:
  // 线程状态
  bool joined_;
  bool started_;

  pthread_t threadId_;
  Functor threadFunc_;
  std::string name_;

  // 线程计数器
  static std::atomic<int> numCreated_;

public:
  Thread(Functor threadFunc, const std::string &name = std::string());
  ~Thread();

  std::string GetName() const { return name_; }
  pthread_t GetThreadId() const { return threadId_; }

  void Start();
  void Join();
};

struct ThreadData {
  Functor threadFunc;
  const std::string threadName;
  pthread_t threadId;

  ThreadData(Functor func, std::string name, pthread_t id)
      : threadFunc(func), threadName(name), threadId(id) {}
};

void *RunTheadFun(void *);

} // namespace web

#endif // _THREAD_H