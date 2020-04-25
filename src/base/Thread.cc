#include "Thread.h"
#include <cassert>

using namespace web;

Thread::Thread(Functor threadFunc, const std::string &name = std::string())
    : joined_(false), started_(false), threadFunc_(threadFunc), name_(name) {
  int num = numCreated_++;
  if (name_.empty()) {
    name_ = "Thread " + std::to_string(num);
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

