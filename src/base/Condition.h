#ifndef WEBSERVER_SRC_BASE_CONDITION_H
#define WEBSERVER_SRC_BASE_CONDITION_H

#include "noncopyable.h"
#include <functional>
#include <pthread.h>

namespace web {
class Mutex;
class Condition : public noncopyable {
private:
  Mutex &mutex_;
  pthread_cond_t cond_;

public:
  explicit Condition(Mutex &mtx);
  ~Condition();

  void wait();
  void waitIf(const std::function<bool()> &cond);
  /*********
   * @brief 让条件变量等待相应的时间
   * @param second 等待的时间
   * @return
   */
  bool waitForSeconds(int second);
  void notify();
  void notifyAll();
};
} // namespace web

#endif // WEBSERVER_SRC_BASE_CONDITION_H