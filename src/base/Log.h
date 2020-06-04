/******
 * @author Ruitao Guo
 * @brief 用户可见的日志接口，内部采用异步日志实现。
 */
#ifndef WEBSERVER_SRC_BASE_LOG_H
#define WEBSERVER_SRC_BASE_LOG_H

#include "noncopyable.h"
#include "Logger.h"
#include "LogStream.h"

namespace web {

enum class LogLevel {
  NONE = 0x00,
  INFO = 0x01,
  DEBUG = 0x01 << 1,
  ERROR = 0x01 << 2,
  FATAL = 0x01 << 3,
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
  LogLevel level_;
  std::string baseName_;
  int line_;

  static std::string logFileName_;

  void formatTime();
};

#define LOG_INFO 0

}

#endif //WEBSERVER_SRC_BASE_LOG_H
