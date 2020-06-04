#ifndef WEBSERVER_SRC_BASE_LOGGER_H
#define WEBSERVER_SRC_BASE_LOGGER_H

#include "../base/noncopyable.h"
#include "../base/Thread.h"
#include "../base/Mutex.h"
#include "../base/Condition.h"
#include "LogBuffer.h"
#include <memory>
#include <deque>
#include <unordered_map>

namespace web {
// 异步日志
class Logger : noncopyable {
public:
  Logger(const std::string &fileName);

  void start();
  void append(const char *line, int len);
  void stop();

private:
  using Buffer = LogBuffer<kLargeSize>;
  using BufferPtr = std::shared_ptr<Buffer>;
  using BufferQueue = std::deque<BufferPtr>;

  bool running_;
  std::string logName_;

  std::unordered_map<Thread::id, BufferPtr> buffers_; // 每个线程对应一个缓冲区
  Mutex bufferMutex_;

  BufferQueue freeBufferQueue_; // 空闲缓冲区
  Mutex freeMutex_;

  BufferQueue flushBufferQueue_; // 刷新缓冲区
  Mutex flushMutex_;
  Condition flushCond_;

  Thread flushThread_; // 后台刷新线程

  void flush();
};

}

#endif //WEBSERVER_SRC_BASE_LOGGER_H
