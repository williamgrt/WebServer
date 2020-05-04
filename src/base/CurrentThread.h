#ifndef _CURRENTTHREAD_H
#define _CURRENTTHREAD_H

namespace web {
namespace CurrentThread {
extern __thread const char *threadName;
} // namespace CurrentThread

} // namespace web

#endif // _CURRENTTHREAD_H