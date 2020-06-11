#ifndef WEBSERVER_SRC_BASE_LOGSTREAM_H
#define WEBSERVER_SRC_BASE_LOGSTREAM_H

#include "noncopyable.h"
#include "LogBuffer.h"
#include <chrono>

namespace web {
const int kMaxNumSize = 12; // 每个数字可以占用的最多字节数
class LogStream : noncopyable {
public:
  using Buffer = LogBuffer<kSmallSize>;

  const Buffer &buffer() { return buffer_; }
  void append(const char *line, std::size_t len);

  LogStream &operator<<(bool flag);
  LogStream &operator<<(short);
  LogStream &operator<<(unsigned short);
  LogStream &operator<<(int);
  LogStream &operator<<(unsigned int);
  LogStream &operator<<(long);
  LogStream &operator<<(unsigned long);
  LogStream &operator<<(long long);
  LogStream &operator<<(unsigned long long);
  LogStream &operator<<(float n);
  LogStream &operator<<(double);
  LogStream &operator<<(long double);
  LogStream &operator<<(char c);
  LogStream &operator<<(char *str);
  LogStream &operator<<(const char *str);
  LogStream &operator<<(const unsigned char *str);
  LogStream &operator<<(const std::string &str);

private:
  Buffer buffer_;
};
}

#endif //WEBSERVER_SRC_BASE_LOGSTREAM_H
