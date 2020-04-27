#ifndef _CONDITION_H
#define _CONDITION_H

#include "noncopyable.h"
#include <functional>
#include <pthread.h>

namespace web {
class Mutex;
class Condition : public noncopyable {
private:
  Mutex &mutex_;
  pthread_cond_t cond_;

public:
  Condition(Mutex &mtx);
  ~Condition();

  void Wait();
  void WaitIf(std::function<bool()> cond);
  void Notify();
  void NotifyAll();
};
} // namespace web

#endif // _CONDITION_H