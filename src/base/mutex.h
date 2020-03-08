//
// Created by 郭睿韬 on 2020/3/7.
//

#ifndef WEBSERVER_SRC_BASE_MUTEX_H_
#define WEBSERVER_SRC_BASE_MUTEX_H_

#include <pthread.h>
#include "noncopyable.h"

namespace server {

class Mutex : noncopyable {
 public:
  // 声明条件变量类为友元
  friend class CondVar;
  // 构造函数
  explicit Mutex() { pthread_mutex_init(&mutex_, nullptr); }
  ~Mutex() {
    pthread_mutex_lock(&mutex_);
    pthread_mutex_destroy(&mutex_);
  }
  // 操作函数
  void lock() { pthread_mutex_lock(&mutex_); }
  void unlock() { pthread_mutex_unlock(&mutex_); }
  pthread_mutex_t *get() { return &mutex_; }

 private:
  pthread_mutex_t mutex_;
};

class LockGuard : noncopyable {
 public:
  explicit LockGuard(server::Mutex &mtx) : mutex_(mtx) {
    mutex_.lock();
  }
  ~LockGuard() { mutex_.unlock(); }

 private:
  server::Mutex &mutex_;
};
}

#endif //WEBSERVER_SRC_BASE_MUTEX_H_
