/*
 * @author: Ruitao Guo
 */

#include "thread.h"

namespace server {

Thread::Thread(const server::Thread::ThreadFunc &thread_func, const std::string &name) :
    started_(false),
    joined_(false),
    threadID_(pthread_t()),
    tid_(0),
    func_(thread_func),
    name_(name) {}

Thread::~Thread() {
  // 主线程有可能先于子线程退出
  // 如果线程已经启动，并且没有调用join
  if (started_ && !joined_) {
    pthread_detach(threadID_);
  }
}

void Thread::start() {
  assert(!started_);
  started_ = true;
  auto t_data = new ThreadData();
  t_data->func_ = func_;
  if (pthread_create(&threadID_, nullptr, startFunc, t_data)) {
    // 创建线程失败
    // 需要释放资源，避免内存泄漏
    delete t_data;
    started_ = false;
  }
}

int Thread::join() {
  assert(started_);
  assert(!joined_);
  joined_ = true;
  return pthread_join(threadID_, nullptr);
}

// 传入pthread_create的参数
// 包裹了实际的线程执行函数
static void *startFunc(void *arg) {
  auto t_data = static_cast<ThreadData *>(arg);
  t_data->func_();
  delete t_data;
  return nullptr;
}

}