#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H

namespace gnet {
class NonCopyable {
public:
  NonCopyable(const NonCopyable &) = delete;
  NonCopyable &operator=(const NonCopyable &) = delete;
};
} // namespace gnet

#endif // _NONCOPYABLE_H