#ifndef WEBSERVER_SRC_BASE_LOGBUFFER_H
#define WEBSERVER_SRC_BASE_LOGBUFFER_H

#include <memory>
#include <cstring>
#include "../base/noncopyable.h"

namespace web {
const int kSmallSize = 4096; // 小缓冲区
const int kLargeSize = 4096 * 1024; // 大缓冲区

// 日志缓冲区
template<int SIZE>
class LogBuffer : noncopyable {
public:
  LogBuffer() : curr_(buffer_) {}

  std::size_t length() const { return curr_ - buffer_; }
  std::size_t available() const { return end() - curr_; }

  char *data() const { return buffer_; }
  char *curr() const { return curr_; }

  void reset() { curr_ = buffer_; }
  void clear() { memset(buffer_, 0, sizeof(buffer_)); }

  void append(const char *line, std::size_t len);
  void add(std::size_t len) { curr_ += len; }

private:
  char *end() const { return buffer_ + sizeof(buffer_); }

  char *curr_; // 当前指向的缓冲区指针
  char buffer_[SIZE]; // 每个 buffer 对象分配固定大小的缓冲区
};

}

#endif //WEBSERVER_SRC_BASE_LOGBUFFER_H
