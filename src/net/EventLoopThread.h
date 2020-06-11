#ifndef WEBSERVER_SRC_NET_EVENTLOOPTHREAD_H
#define WEBSERVER_SRC_NET_EVENTLOOPTHREAD_H

#include "../base/noncopyable.h"
namespace web {
class EventLoopThread: noncopyable {
public:
  EventLoopThread();
  ~EventLoopThread();

};
}

#endif //WEBSERVER_SRC_NET_EVENTLOOPTHREAD_H
