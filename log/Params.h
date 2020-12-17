#ifndef WEBSERVER_NET_PARAMS_H_
#define WEBSERVER_NET_PARAMS_H_

namespace gweb {
struct Params {
  // 最多监听事件的数量
  static const int MAX_EVENT_NUM = 1024;
  // poller等待的最大时间
  static const int WAIT_MS = -1;
  // 线程池的线程个数
  static const int THREAD_NUM = 1;
  // listen backlog
  static const int BACKLOG = 1024;
  // 从reactor的数量
  static const int REACTOR_NUM = 1;
  // 缓冲区缓冲块数量
  static const int BUF_LEN = 4194304;
};

}

#endif //WEBSERVER_NET_PARAMS_H_
