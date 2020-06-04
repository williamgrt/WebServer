#ifndef WEBSERVER_SRC_BASE_CURRENTTHREAD_H
#define WEBSERVER_SRC_BASE_CURRENTTHREAD_H

namespace web {
// 提前缓存线程相关的信息
// 在日志模块打印线程信息时有用
namespace CurrentThread {
extern thread_local const char *tName; // 线程名
extern thread_local char tidStr[32];
extern thread_local int tidStrLen;
extern thread_local int tid;

void cacheTid();

inline const char *getName() {
  return tName;
}

// 获取当前线程 id
inline int getId() {
  if (__builtin_expect(tid == 0, 0)) {
    cacheTid();
  }
  return tid;
}

} // namespace CurrentThread

} // namespace web

#endif // WEBSERVER_SRC_BASE_CURRENTTHREAD_H