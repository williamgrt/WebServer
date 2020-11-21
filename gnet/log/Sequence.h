#ifndef WEBSERVER_UTILS_SEQUENCE_H_
#define WEBSERVER_UTILS_SEQUENCE_H_

#include <atomic>

#define CACHE_LINE_BYTES_SIZE 64

// 对atomic_int64_t的封装，进行内存补齐，保证每个变量独占一个缓存行，避免伪共享的情况发生
class Sequence {
 public:
  explicit Sequence(const int64_t number = 0L): val_(number) {}
  ~Sequence() = default;

  void store(const int64_t val) { val_.store(val); }
  int64_t load() { return val_.load(); }
  int64_t fetch_add(const int64_t i) { return val_.fetch_add(i); }

 private:
  // 内存补齐，保证每个序号独占一条缓存行
  std::atomic_int64_t val_;
  char padding_[CACHE_LINE_BYTES_SIZE - sizeof(std::atomic_int64_t)];
};

#endif //WEBSERVER_UTILS_SEQUENCE_H_
