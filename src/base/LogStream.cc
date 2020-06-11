#include "LogStream.h"

using namespace web;
using namespace std;

void LogStream::append(const char *line, std::size_t len) {
  buffer_.append(line, len);
}

LogStream &LogStream::operator<<(bool flag) {
  buffer_.append(flag ? "1" : "0", 1);
  return *this;
}

LogStream &LogStream::operator<<(short value) {
  *this << static_cast<int>(value);
  return *this;
}

LogStream &LogStream::operator<<(unsigned short value) {
  *this << static_cast<unsigned int>(value);
  return *this;
}

LogStream &LogStream::operator<<(int value) {
  if (buffer_.available() > kMaxNumSize) {
    std::size_t size = ::snprintf(buffer_.curr(), kMaxNumSize, "%d", value);
    buffer_.add(size);
  }
  return *this;
}

LogStream &LogStream::operator<<(unsigned int value) {
  if (buffer_.available() > kMaxNumSize) {
    std::size_t size = snprintf(buffer_.curr(), kMaxNumSize, "%u", value);
    buffer_.add(size);
  }
  return *this;
}

LogStream &LogStream::operator<<(long value) {
  if (buffer_.available() > kMaxNumSize) {
    std::size_t size = snprintf(buffer_.curr(), kMaxNumSize, "%ld", value);
    buffer_.add(size);
  }
  return *this;
}

LogStream &LogStream::operator<<(unsigned long value) {
  if (buffer_.available() > kMaxNumSize) {
    std::size_t size = snprintf(buffer_.curr(), kMaxNumSize, "%lu", value);
    buffer_.add(size);
  }
  return *this;
}

LogStream &LogStream::operator<<(long long int value) {
  if (buffer_.available() > kMaxNumSize) {
    std::size_t size = snprintf(buffer_.curr(), kMaxNumSize, "%lld", value);
    buffer_.add(size);
  }
  return *this;
}

LogStream &LogStream::operator<<(unsigned long long int value) {
  if (buffer_.available() > kMaxNumSize) {
    std::size_t size = snprintf(buffer_.curr(), kMaxNumSize, "%llu", value);
    buffer_.add(size);
  }
  return *this;
}

LogStream &LogStream::operator<<(float n) {
  *this << static_cast<double >(n);
  return *this;
}

LogStream &LogStream::operator<<(double value) {
  if (buffer_.available() > kMaxNumSize) {
    std::size_t size = snprintf(buffer_.curr(), kMaxNumSize, "%.12g", value);
    buffer_.add(size);
  }
  return *this;
}

LogStream &LogStream::operator<<(long double value) {
  if (buffer_.available() > kMaxNumSize) {
    std::size_t size = snprintf(buffer_.curr(), kMaxNumSize, "%.12Lg", value);
    buffer_.add(size);
  }
  return *this;
}

LogStream &LogStream::operator<<(char c) {
  buffer_.append(const_cast<const char *>(&c), 1);
  return *this;
}

LogStream &LogStream::operator<<(char *str) {
  if (str) {
    buffer_.append(str, strlen(str));
  }
  return *this;
}

LogStream &LogStream::operator<<(const char *str) {
  if (str) {
    buffer_.append(str, strlen(str));
  }
  return *this;
}

LogStream &LogStream::operator<<(const unsigned char *str) {
  *this << reinterpret_cast<const char *>(str);
  return *this;
}

LogStream &LogStream::operator<<(const string &str) {
  buffer_.append(str.c_str(), str.size());
  return *this;
}
