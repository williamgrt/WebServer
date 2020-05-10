#ifndef WEBSERVER_SRC_BASE_MUTEX_H
#define WEBSERVER_SRC_BASE_MUTEX_H

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
  pthread_mutex_t mutex_;

public:
  Mutex();
  ~Mutex();

  pthread_mutex_t *getPthreadMutex() { return &mutex_; }

  void lock();
  void unlock();
};

class LockGuard {
private:
  Mutex &mutex_;

public:
  LockGuard(Mutex &mutex) : mutex_(mutex) { mutex.lock(); }
  ~LockGuard() { mutex_.unlock(); }
};

} // namespace web

#endif // WEBSERVER_SRC_BASE_MUTEX_H