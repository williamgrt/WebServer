#ifndef _MUTEX_H
#define _MUTEX_H

#include "noncopyable.h"
#include <pthread.h>

#define MCHECK(ret)                                                            \
  ({                                                                           \
    __typeof__(ret) errnum = (ret);                                            \
    assert(errnum == 0);                                                       \
    (void)errnum;                                                              \
  })

namespace web {
class Mutex : public noncopyable {
private:
  pthread_mutex_t mtx_;

public:
  Mutex();
  ~Mutex();

  pthread_mutex_t *GetPthreadMutex() { return &mtx_; }

  void Lock();
  void Unlock();
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