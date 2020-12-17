#include "Buffer.h"
#include <sys/uio.h>
#include <cerrno>
#include <cassert>

using namespace gweb;
using namespace std;

Buffer::Buffer(size_t init) :
    buffer_(vector<char>(1024)),
    readIndex_(kPrepareSize),
    writeIndex_(kPrepareSize) {}

size_t Buffer::readFromFd(int fd, int *error) {
  // 使用readv读取
  // TODO: 为什么更好？解释原因
  // 增加一块位于栈的缓冲区，基本可以满足读取需求
  char extraBuf[65536];
  struct iovec vec[2];
  const size_t writeable = writeableBytes();
  vec[0].iov_base = begin() + writeIndex_;
  vec[0].iov_len = writeable;
  vec[1].iov_base = extraBuf;
  vec[1].iov_len = sizeof(extraBuf);

  const ssize_t n = readv(fd, vec, 2);
  if (n < 0) {
    *error = errno;
  } else if (n <= writeable) {
    writeIndex_ += n;
  } else {
    // 当前的缓冲区不够了，需要分配新的缓冲区
    writeIndex_ = buffer_.size();
    append(extraBuf, n - writeable);
  }
  return n;
}

void Buffer::append(const char *data, size_t len) {
  // 确认写入数据的空间是够用的
  ensureSize(len);
  // 拷贝数据
  std::copy(data, data + len, beginToWrite());
  // 更新参数
  hasWrite(len);
}

void Buffer::ensureSize(size_t len) {
  // 实现的是最简单的resize，以后根据需要修改
  buffer_.resize(writeIndex_ + len);
}

void Buffer::retrieve(size_t len) {
  assert(len <= readableBytes());
  if (len < readableBytes()) {
    // 没读完
    readIndex_ += len;
  } else {
    // 读完了
    clear();
  }
}

string Buffer::retrieveAsString() {
  string result(buffer_.data(), readableBytes());
  clear();
  return result;
}

