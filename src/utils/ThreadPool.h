#ifndef WEBSERVER_SRC_BASE_THREADPOOL_H_
#define WEBSERVER_SRC_BASE_THREADPOOL_H_

#include <functional>
#include <vector>
#include <deque>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "noncopyable.h"

namespace web {
/*
 * 使用多生产者消费者模型实现的线程池。
 * 内部包含一个互斥锁，两个条件变量。（原因查看《ostep》第 30 章）
 */
class ThreadPool : public noncopyable {
 public:
  using Task = std::function<void()>;

  ThreadPool(int maxTaskNum, int threadNum);
  ~ThreadPool();

  // 线程池的控制函数
  void start();
  void stop();

  void setMaxTaskNum(int maxTaskNum);
  void setThreadNum(int threadNum);
  void addTask(Task task);

 private:
  // 线程池中每个线程实际执行的函数
  void runInThread();

  bool isEmpty() {
    return tasks_.empty();
  }

  bool isFull() {
    return tasks_.size() >= maxTaskNum_;
  }

  bool running_;
  size_t maxTaskNum_;
  int threadNum_;

  std::mutex mutex_;
  std::condition_variable empty_;
  std::condition_variable full_;
  std::vector<std::unique_ptr<std::thread>> threads_;
  std::deque<Task> tasks_;
};
}

#endif //WEBSERVER_SRC_BASE_THREADPOOL_H_
