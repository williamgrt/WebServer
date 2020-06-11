/******
 * @author Ruitao Guo
 * @brief 用户可见的日志接口，内部采用异步日志实现。
 */
#ifndef WEBSERVER_SRC_BASE_LOG_H
#define WEBSERVER_SRC_BASE_LOG_H

#include "noncopyable.h"
#include "LogStream.h"

namespace web {

enum class LogLevel {
  TRACE = 0x01,
  DEBUG = 0x01 << 1,
  INFO = 0x01 << 2,
  WARN = 0x01 << 3,
  ERROR = 0x01 << 4,
  FATAL = 0x01 << 5,
  ALL = 0x7fffffff
};

// 用户日志接口
class Log : noncopyable {
public:
  Log(const char *name, int line);
  ~Log();

  LogStream &stream() { return stream_; }

  static void setLogFileName(const std::string &name) { logFileName_ = name; }
  static std::string getLogFileName() { return logFileName_; }

private:
  LogStream stream_;
  std::string baseName_;
  int line_;
  LogLevel level_;
  // 所有的日志都写入到同一个文件中
  static std::string logFileName_;

  void formatTime();
};

#define LOG_INFO Log(__FILE__, __LINE__).stream()
#define LOG_DEBUG Log(__FILE__, __LINE__).stream()

}

#endif //WEBSERVER_SRC_BASE_LOG_H
