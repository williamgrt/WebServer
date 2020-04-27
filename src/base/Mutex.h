#ifndef _MUTEX_H
#define _MUTEX_H

#include "noncopyable.h"
#include <pthread.h>

namespace web {
class Mutex : public noncopyable {
private:
  pthread_mutex_t mtx_;

public:
  Mutex();
  ~Mutex();

  void Lock();
  void Unlock();

  friend class Condition; // 条件变量能够访问互斥锁的私有变量
};

class LockGuard {
private:
  Mutex &mutex_;

public:
  LockGuard(Mutex &mutex) : mutex_(mutex) { mutex.Lock(); }
  ~LockGuard() { mutex_.Unlock(); }
};

} // namespace web

#endif // _MUTEX_H