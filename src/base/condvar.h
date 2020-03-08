//
// Created by 郭睿韬 on 2020/3/7.
//

#ifndef WEBSERVER_SRC_BASE_CONDVAR_H_
#define WEBSERVER_SRC_BASE_CONDVAR_H_

#include <pthread.h>
#include "noncopyable.h"
#include "mutex.h"

namespace server {

class CondVar : noncopyable {
 public:
  // 构造函数
  CondVar(server::Mutex &mtx) : mutex_(mtx) {
    pthread_cond_init(&cond, nullptr);
  }
  ~CondVar() { pthread_cond_destroy(&cond); }
  // 操作函数
  void wait() { pthread_cond_wait(&cond, mutex_.get()); }
  // void wait(server::Mutex *mutex_) { pthread_cond_wait(&cond, &mutex_->mutex_); }
  // 不建议这样写
  // 有可能每次conditional variable 每次调用不会绑定同一个Mutex
  void notify() { pthread_cond_signal(&cond); }
  void notifyAll() { pthread_cond_broadcast(&cond); }

 private:
  server::Mutex &mutex_;
  pthread_cond_t cond;
};
}

#endif //WEBSERVER_SRC_BASE_CONDVAR_H_
