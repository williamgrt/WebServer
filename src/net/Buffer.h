#ifndef WEBSERVER_SRC_NET_BUFFER_H
#define WEBSERVER_SRC_NET_BUFFER_H

#include <vector>
#include <unistd.h>
#include <string>
#include "../base/noncopyable.h"

namespace web {
class Buffer : noncopyable {
public:
  static const size_t kInitSize = 1024;
  static const size_t kPrepareSize = 0; // 包头的保留字节（暂时为0）

  Buffer(size_t init = kInitSize);
  ~Buffer() = default;

  /* ------------ 获取缓冲区相关信息 ------------ */

  size_t readableBytes() { return writeIndex_ - readIndex_; }
  size_t writeableBytes() { return buffer_.size() - writeIndex_; }
  bool isEmpty() { return readableBytes() == 0; }
  // 获得可写的起始位置
  char *beginToWrite() { return buffer_.data() + writeIndex_; }
  // 获得可读的起始位置
  char *beginToRead() { return buffer_.data() + readIndex_; }

  /* ------------ 缓冲区读操作 ------------ */

  void retrieve(size_t len);
  /*****
   * @brief 读取缓冲区可读部分，返回缓冲区可读内容
   * @return 缓冲区的可读内容
   */
  std::string retrieveAsString();

  /* ------------ 缓冲区写操作 ------------ */

  /*****
   * @brief
   * @param len
   */
  void hasWrite(size_t len) { writeIndex_ += len; }
  // 往缓冲区中添加数据
  void append(const char *data, size_t len);
  void append(const std::string &data) {
    append(data.c_str(), data.size());
  }

  // 扩展缓冲区，保证可以存放下所有的数据
  void ensureSize(size_t len);
  // 清空缓冲区
  // 更改指针操作即可
  void clear() {
    readIndex_ = kPrepareSize;
    writeIndex_ = kPrepareSize;
  }
  // 从套接字fd中读取内容
  size_t readFrom(int fd, int *error);

private:
  char *begin() { return buffer_.data(); }

  // 使用vector管理buffer（方便resize）
  std::vector<char> buffer_;
  size_t readIndex_;
  size_t writeIndex_;

};

}

#endif //WEBSERVER_SRC_NET_BUFFER_H
