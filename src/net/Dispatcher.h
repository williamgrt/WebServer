//
// Created by 郭睿韬 on 2020/5/25.
//

#ifndef WEBSERVER_SRC_BASE_DISPATCHER_H
#define WEBSERVER_SRC_BASE_DISPATCHER_H

#include "../base/noncopyable.h"
#include <memory>

namespace web {

class Channel;
// 事件分发器的抽象接口
// 在 Linux 上使用 epoll，其他 Unix 系统上使用 poll
class Dispatcher : noncopyable {

  // 分为 epoll 和 poll 两种情况，全部实现为虚函数
  virtual void addChannel(Channel *) = 0;
  virtual void modifyChannel(Channel *) = 0;
  virtual void removeChannel(Channel *) = 0;

};
}

#endif //WEBSERVER_SRC_BASE_DISPATCHER_H
