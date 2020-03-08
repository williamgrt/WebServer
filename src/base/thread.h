//
// Created by 郭睿韬 on 2020/3/6.
//

#ifndef WEBSERVER_SRC_BASE_THREAD_H
#define WEBSERVER_SRC_BASE_THREAD_H

#include "noncopyable.h"

#include <pthread.h>
#include <functional>
#include <string>

namespace server {

struct ThreadData;
class Thread : noncopyable {
 public:
  // 线程执行函数
  using ThreadFunc = std::function<void()>;
  // 构造与析构函数
  explicit Thread(const ThreadFunc&, const std::string &name = std::string());
  ~Thread();
  // 线程控制函数
  void start();
  int join();
  // 线程状态函数
  bool isStarted() const { return started_; }
  bool isJoined() const { return joined_; }

 private:
  // 线程状态
  bool started_;
  bool joined_;
  // 线程数据
  pthread_t threadID_;
  pid_t tid_;
  Thread::ThreadFunc func_;
  std::string name_;
};

// 对线程实际运行函数的封装
struct ThreadData : noncopyable {
  Thread::ThreadFunc func_;
};

static void *startFunc(void *);
}

#endif //WEBSERVER_SRC_BASE_THREAD_H
