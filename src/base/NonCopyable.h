#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H

namespace gnet {
// 不可复制的类
class NonCopyable {
public:
  NonCopyable();
  ~NonCopyable();

  NonCopyable(const NonCopyable &) = delete;
  NonCopyable &operator=(const NonCopyable &) = delete;
};
} // namespace gnet

#endif // _NONCOPYABLE_H