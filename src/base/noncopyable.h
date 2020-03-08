//
// Created by 郭睿韬 on 2020/3/6.
//

#ifndef WEBSERVER_SRC_BASE_NONCOPYABLE_H_
#define WEBSERVER_SRC_BASE_NONCOPYABLE_H_

namespace server {
class noncopyable {
 public:
  noncopyable(const noncopyable &) = delete;
  void operator=(const noncopyable &) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};
}

#endif //WEBSERVER_SRC_BASE_NONCOPYABLE_H_
