#ifndef WEBSERVER_SRC_BASE_NONCOPYABLE_H
#define WEBSERVER_SRC_BASE_NONCOPYABLE_H

namespace web {
// 不可复制的类
class noncopyable {
public:
  noncopyable(){};
  ~noncopyable(){};

  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;
};
} // namespace web

#endif // WEBSERVER_SRC_BASE_NONCOPYABLE_H