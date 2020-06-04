#include "MmapFile.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <error.h>

using namespace web;
using namespace std;

const int MmapFile::kInvalid = -1;
char *MmapFile::invalidAddr = reinterpret_cast<char *>(-1);
const std::size_t MmapFile::kDefaultSize = 4 * 1024 * 1024;

MmapFile::MmapFile() : fd_(kInvalid),
                       memory_(invalidAddr),
                       offset_(0),
                       size_(0),
                       sync_(0) {}

MmapFile::~MmapFile() {
  close();
}

void MmapFile::open(const char *name, bool append) {
  // 首先关闭上一次的文件和映射
  close();

  fd_ = ::open(name, O_RDWR | O_CREAT | (append ? O_APPEND : 0), (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
  if (fd_ == kInvalid) {
    ::error(1, errno, "open log file error.");
  }

  if (append) {
    // 文件的写入方式是往尾部添加数据
    // 因此需要调整类内数据偏移量if
      struct stat st{};
    ::fstat(fd_, &st);
    size_ = std::max<std::size_t>(kDefaultSize, st.st_size);
    offset_ = st.st_size;
  } else {
    size_ = kDefaultSize;
    offset_ = 0;
  }

  // 调整文件的大小
  int r = ::ftruncate(fd_, size_);
  assert(r == 0);

  mapFileToMemory();
}

void MmapFile::open(const string &name, bool append) {
  open(name.c_str(), append);
}

void MmapFile::close() {
  if (fd_ != kInvalid) {
    // 关闭文件以及存储映射区
    ::close(fd_);
    ::munmap(memory_, size_);
    // 重设类内变量
    fd_ = kInvalid;
    memory_ = invalidAddr;
    offset_ = 0;
    size_ = 0;
    sync_ = 0;
  }
}

void MmapFile::sync() {
  if (fd_ == kInvalid || memory_ == invalidAddr) {
    return;
  }
  // 保证日志可以被操作系统写入到磁盘中
  int r = ::msync(memory_ + sync_, offset_ - sync_, MS_ASYNC);
  if (r < 0) {
    error(fileno(stderr), errno, "flush log to file error.");
  }
  sync_ = offset_;
}

void MmapFile::write(const char *line, size_t len) {
  // 保证文件有足够的容量写
  expendSpace(len);

  ::memcpy(memory_ + offset_, line, len);
  offset_ += len;
}

void MmapFile::truncate(std::size_t size) {
  if (size == size_) {
    return;
  }

  std::size_t oldSize = size_;
  size_ = size;
  int r = ::ftruncate(fd_, size_);
  if (r < 0) {
    error(fileno(stderr), errno, "truncate log file error.");
    abort();
  }

  if (offset_ > size_) {
    offset_ = size_;
  }

  // 重新映射文件
  ::munmap(memory_, oldSize);
  memory_ = invalidAddr;
  mapFileToMemory();
}

bool MmapFile::mapFileToMemory() {
  if (fd_ == kInvalid || size_ == 0) {
    return false;
  }

  memory_ = (char *)::mmap(0, size_, PROT_WRITE, MAP_SHARED, fd_, 0);
  return (memory_ != invalidAddr);
}

// 给写入文件预留足够空间
void MmapFile::expendSpace(std::size_t len) {
  if (fd_ == kInvalid) {
    return;
  }

  // 调整内存映射大小
  std::size_t newSize = size_;
  while (offset_ + len < newSize) {
    newSize += kDefaultSize;
  }

  if (newSize > size_) {
    truncate(newSize);
  }
}