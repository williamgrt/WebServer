#include "Thread.h"
#include "CurrentThread.h"
#include <cassert>
#include <syscall.h>
#include <unistd.h>
#include <sys/prctl.h>

using namespace web;

namespace web {
namespace CurrentThread {
thread_local const char *threadName = "default";
}
}

void *runThreadFunc(void *arg) {
  ThreadData *data = static_cast<ThreadData *>(arg);
  data->runThread();
  delete data;
  return nullptr;
}

void ThreadData::runThread() {
  // 设置运行函数的名字
  CurrentThread::threadName = name_.empty() ? "GnetThread" : name_.c_str();
  prctl(PR_SET_NAME, CurrentThread::threadName);
  // 运行函数
  func_();
  // 把当前线程的名字改为finished
  web::CurrentThread::threadName = "finished";
}

std::atomic<int> Thread::numGenerated_(0); // 初始化程序计数器

Thread::Thread(Functor threadFunc, const std::string &name)
    : joined_(false),
    started_(false),
    threadFunc_(std::move(threadFunc)),
    name_(std::move(name)) {
  int num = numGenerated_++;
  if (name_.empty()) {
    name_ = "GWebThread " + std::to_string(num);
  }
}

Thread::~Thread() {
  // 当Thread生命周期结束时，线程还在运行
  // 如果当前线程正在运行，并且没有执行join操作，直接析构会出现僵尸线程的情况
  // 因此需要调用pthread_detach
  if (started_ && !joined_) {
    pthread_detach(threadId_);
  }
}

void Thread::join() {
  assert(started_);
  assert(!joined_);

  joined_ = true;
  int r = pthread_join(threadId_, nullptr);
  if (r == -1) {
    perror("process join error.");
  }
}

void Thread::start() {
  assert(!started_);

  started_ = true;
  ThreadData *data = new ThreadData(threadFunc_, name_, threadId_);
  int r = pthread_create(&threadId_, nullptr, runThreadFunc, (void *) data);
  if (r == 0) {
    // TODO: handle the success of pthread create
  } else {
    // pthread_create失败
    // 删除动态分配的内存
    delete data;
    // 重置运行状态
    started_ = false;
  }
}

pid_t Thread::gettid() {
  return syscall(SYS_gettid);
}
