#ifndef WEBSERVER_SRC_BASE_TEST_MMAPFILE_H
#define WEBSERVER_SRC_BASE_TEST_MMAPFILE_H

#include <string>
#include "noncopyable.h"
namespace web {
// 写入文件的类
class MmapFile : noncopyable {
public:
  MmapFile();
  ~MmapFile();

  void open(const char *name, bool append = true);
  void open(const std::string &name, bool append = true);
  void close();
  void sync();

  void write(const char *line, size_t len);
  void truncate(std::size_t len);

  std::size_t size() const { return size_; }
  bool isOpen() const { return fd_ != kInvalid; }

private:
  int fd_;
  char *memory_;
  std::size_t offset_;
  std::size_t size_;
  std::size_t sync_;

  static const int kInvalid;
  static char *invalidAddr;
  static const std::size_t kDefaultSize;

  void expendSpace(std::size_t len);
  bool mapFileToMemory();
};

}


#endif //WEBSERVER_SRC_BASE_TEST_MMAPFILE_H
