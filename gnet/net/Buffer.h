#ifndef WEBSERVER_SRC_NET_BUFFER_H
#define WEBSERVER_SRC_NET_BUFFER_H

#include <vector>
#include <unistd.h>
#include <string>

#include <gnet/utils/noncopyable.h>

namespace gweb {
class Buffer : noncopyable {
public:
  static const size_t kInitSize = 1024;
  static const size_t kPrepareSize = 0; // 包头的保留字节（暂时为0）

  explicit Buffer(size_t init = kInitSize);
  ~Buffer() = default;

  /* ------------ 获取缓冲区相关信息 ------------ */

  // 可读的内容长度
  size_t readableBytes() { return writeIndex_ - readIndex_; }
  // 可写的内容长度
  size_t writeableBytes() { return buffer_.size() - writeIndex_; }
  // 缓冲区是否为空（没有可写的内容）
  bool isEmpty() { return readableBytes() == 0; }
  // 获得可写的起始位置
  char *beginToWrite() { return buffer_.data() + writeIndex_; }
  // 获得可读的起始位置
  char *beginToRead() { return buffer_.data() + readIndex_; }

  /* ------------ 缓冲区读操作 ------------ */

  //
  void retrieve(size_t len);
  // 获得缓冲区所有可写的内容
  std::string retrieveAsString();

  /* ------------ 缓冲区写操作 ------------ */

  /*****
   * @brief 调整缓存写入数据的指针
   * @param len 写入数据的长度
   */
  void hasWrite(size_t len) { writeIndex_ += len; }
  // 往缓冲区写入执行长度的内容
  void append(const char *data, size_t len);
  /*****
   * @brief 往缓冲区中写入数据，无需指定长度
   * @param data 数据内容
   */
  void append(const std::string &data) {
    append(data.c_str(), data.size());
  }

  // 清空缓冲区
  void clear() {
    readIndex_ = kPrepareSize;
    writeIndex_ = kPrepareSize;
  }
  // 从套接字fd中读取内容
  size_t readFromFd(int fd, int *error);

private:
  // 保证缓冲区有足够的内容写入
  void ensureSize(size_t len);
  // 获取起始位置
  char *begin() { return buffer_.data(); }

  // 使用vector管理buffer（方便resize）
  std::vector<char> buffer_;
  size_t readIndex_;
  size_t writeIndex_;

};

}

#endif //WEBSERVER_SRC_NET_BUFFER_H
