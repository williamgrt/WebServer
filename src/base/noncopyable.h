#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H

namespace web {
// 不可复制的类
class noncopyable {
public:
  noncopyable();
  ~noncopyable();

  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;
};
} // namespace web

#endif // _NONCOPYABLE_H