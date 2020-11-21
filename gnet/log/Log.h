#ifndef WEBSERVER_UTILS_LOG_H_
#define WEBSERVER_UTILS_LOG_H_

#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

#include <gnet/utils/noncopyable.h>
#include <gnet/log/Sequence.h>
#include "Time.h"
#include "Singleton.h"

namespace gweb {
// 日志级别
enum Level {
  INFO = 0,
  DEBUG,
  WARNING,
  ERROR,
  FATAL,
  NUM_LOG_LEVELS,
};

// 异步日志
// 设计思路如下：
//
class Log: noncopyable {
 public:
  Log();
  ~Log();

  // 日志写入函数
  void write(Level level, int val);
  void write(Level level, unsigned val);
  void write(Level level, const char *log);
  void write(Level level, char c);
  void write(Level level, const std::string &log);

  static Log* getInstance() {
    return Singleton<Log>::getInstance();
  }

 private:
  // 把日志写入到缓冲区中
  void write(const std::string &log);
  // 更新缓存时间
  void updateTime();
  // 后台写入日志
  void flushToDisk();

  std::vector<std::string> ringBuffer_; // 环形缓存
  std::unique_ptr<std::thread> backThread_; // 后台线程
  volatile TimeType currSec_; // 现在的时间
  std::string timeStr_; // 缓存的时间字符串
  FILE *file_; // 日志文件
  std::atomic_bool stop_;
  Sequence lastRead_; // 最后一个读的位置
  Sequence lastWrite_; // 最后一个写入的位置
  std::mutex mutex_;
  std::condition_variable condVar_;
  // 所有日志级别对应的字符串
  const std::string levelString[Level::NUM_LOG_LEVELS] = {
      "INFO ",
      "DEBUG ",
      "WARING ",
      "ERROR ",
      "FATAL "
  };
};

// 日志使用函数
#define LOG_INFO(...) Log::getInstance()->write(gweb::Level::INFO, __VA_ARGS__)
#define LOG_DEBUG(...) Log::getInstance()->write(gweb::Level::DEBUG, __VA_ARGS__)
#define LOG_WARNING(...) Log::getInstance()->write(gweb::Level::WARNING, __VA_ARGS__)
#define LOG_ERROR(...) Log::getInstance()->write(gweb::Level::ERROR, __VA_ARGS__)
#define LOG_FATAL(...) Log::getInstance()->write(gweb::Level::FATAL, __VA_ARGS__)
}
#endif //WEBSERVER_UTILS_LOG_H_
