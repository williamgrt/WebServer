#ifndef WEBSERVER_SRC_BASE_LOGGER_H
#define WEBSERVER_SRC_BASE_LOGGER_H

#include "noncopyable.h"
#include "LogBuffer.h"
#include "CountDownLatch.h"
#include <memory>
#include <deque>
#include <unordered_map>
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <queue>

namespace web {
// 异步日志
class Logger : noncopyable {
public:
  explicit Logger(const std::string &fileName);
  ~Logger();

  void start();
  void append(const char *line, std::size_t len);
  void stop();

private:
  using Buffer = LogBuffer<kLargeSize>;
  using BufferPtr = std::unique_ptr<Buffer>;
  using BufferVector = std::vector<BufferPtr>;

  static const int kEntryNum = 12;

  std::string logName_;
  bool running_;

  BufferVector freeBuffers_;
  BufferVector fullBuffers_;

  std::mutex mutex_;
  std::condition_variable cond_;

  // 把锁分为细粒度的锁
  struct Entry {
    std::mutex mutex;
    std::unordered_map<std::thread::id, BufferPtr> buffers;
  };

  std::vector<Entry> tidToMap_;

  std::thread flushThread_; // 后台刷新线程
  CountDownLatch latch_;

  void flush();
};

}

#endif //WEBSERVER_SRC_BASE_LOGGER_H
