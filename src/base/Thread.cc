#include "Thread.h"
#include "CurrentThread.h"
#include <cassert>

using namespace web;

namespace CurrentThread {
__thread const char *threadName = "default";
}

void *RunTheadFunc(void *arg) {
  ThreadData *data = static_cast<ThreadData *>(arg);
  data->RunThread();
  delete data;
  return nullptr;
}

void ThreadData::RunThread() {
  CurrentThread::threadName = name_.empty() ? "GwebThread" : name_.c_str();
  // 真正运行的函数
  func_();
  CurrentThread::threadName = "finished";
}

std::atomic<int> Thread::numGenerated_(0);

Thread::Thread(Functor threadFunc, const std::string &name)
    : joined_(false), started_(false), threadFunc_(threadFunc), name_(name) {
  int num = numGenerated_++;
  if (name_.empty()) {
    name_ = "GWebThread " + std::to_string(num);
  }
}

Thread::~Thread() {
  // 当Thread生命周期结束时，线程还在运行
  if (started_ && !joined_) {
    pthread_detach(threadId_);
  }
}

void Thread::Join() {
  assert(started_);
  assert(!joined_);

  int r = pthread_join(threadId_, nullptr);
  if (r == -1) {
    perror("process join error.");
  }
}

void Thread::Start() {
  assert(!started_);

  started_ = true;
  ThreadData *data = new ThreadData(threadFunc_, name_, threadId_);
  int r = pthread_create(&threadId_, nullptr, RunTheadFunc, (void *)data);
  if (r == 0) {
    // pthread_create成功
  } else {
    // pthread_create失败
    // 删除动态分配的内存
    delete data;
    // 重置运行状态
    started_ = false;
  }
}
