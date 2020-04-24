#ifndef _THREAD_H
#define _THREAD_H

#include "noncopyable.h"
#include <functional>
#include <pthread.h>
#include <string>

namespace web {
using Functor = std::function<void()>;

class Thread : public noncopyable {
private:
  bool joined_;
  bool detached_;
  pthread_t tid_;
  Functor threadFunc_;
  std::string name_;

public:
  Thread(const std::string &name = "");
  ~Thread();
};

} // namespace web

#endif // _THREAD_H