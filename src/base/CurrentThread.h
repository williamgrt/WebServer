#ifndef WEBSERVER_SRC_BASE_CURRENTTHREAD_H
#define WEBSERVER_SRC_BASE_CURRENTTHREAD_H

namespace web {
namespace CurrentThread {
extern __thread const char *threadName;

inline const char *getName() {
  return threadName;
}
} // namespace CurrentThread

} // namespace web

#endif // WEBSERVER_SRC_BASE_CURRENTTHREAD_H