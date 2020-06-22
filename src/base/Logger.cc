#include "Logger.h"
#include "MmapFile.h"
#include "File.h"
#include <memory>
#include <vector>
#include <cassert>
#include <chrono>

using namespace web;
using namespace std;
using namespace std::chrono_literals;

Logger::Logger(const std::string &fileName)
    : logName_(fileName),
      running_(false),
      mutex_(),
      cond_(),
      tidToMap_(vector<Entry>(kEntryNum)),
      flushThread_(),
      latch_(1) {
  assert(logName_.size() > 1);
  freeBuffers_.reserve(16); // 预先分配缓冲区
  // 提前访问缓冲区，避免写入日志时产生 page fault
  // 是否有用？（实验效果随机性较强，可能跟mmap有关）
  // 做了实验发现关联不大，可能有其他的改进措施
  for (auto &buffer: freeBuffers_) {
    buffer->clear();
  }
}

Logger::~Logger() {
  if (running_) {
    stop();
  }
}

void Logger::start() {
  running_ = true;
  flushThread_ = thread(&Logger::flush, this);
  latch_.await();
}

void Logger::append(const char *line, std::size_t len) {
  thread::id tid = this_thread::get_id();
  auto key = hash<thread::id>()(tid) % kEntryNum; // 获得线程哈希对应的桶

  // 获取当前线程的缓冲区
  Buffer *currentBuffer;
  {
    // 锁住单独一个桶
    std::lock_guard<mutex> guard1(tidToMap_[key].mutex);
    auto &bucket = tidToMap_[key].buffers;
    auto iter = bucket.find(tid);
    if (iter == bucket.end()) {
      {
        lock_guard<mutex> guard2(mutex_);
        if (!freeBuffers_.empty()) {
          bucket[tid] = std::move(freeBuffers_.front());
          freeBuffers_.erase(freeBuffers_.begin());
        } else {
          bucket[tid] = make_unique<Buffer>();
        }
      }
    }
    currentBuffer = bucket[tid].get();
  }
  assert(currentBuffer);

  if (currentBuffer->available() > len) {
    currentBuffer->append(line, len);
  } else {
    // 需要找一块新的缓冲区
    {
      std::lock_guard<mutex> guard(tidToMap_[key].mutex);
      auto &bucket = tidToMap_[key].buffers;
      {
        std::lock_guard<mutex> lock(mutex_);
        fullBuffers_.push_back(std::move(bucket[tid]));
        bucket[tid].reset();
        if (!freeBuffers_.empty()) {
          bucket[tid] = std::move(freeBuffers_.front());
          freeBuffers_.erase(freeBuffers_.begin());
        } else {
          bucket[tid] = make_unique<Buffer>();
        }
      }
      cond_.notify_one();
      currentBuffer = bucket[tid].get();
    }
    assert(currentBuffer);

    currentBuffer->append(line, len);
  }
}

void Logger::stop() {
  running_ = false;
  cond_.notify_all();
  flushThread_.join();
}

void Logger::flush() {
  // 打开需要写入的日志文件
  File file(logName_, 3);
  BufferVector buffersToWrite;
  latch_.countDown(); // 等待后端线程准备完成

  while (running_) {
    {
      unique_lock<mutex> lock(mutex_);
      while (running_ && fullBuffers_.empty()) {
        cond_.wait(lock);
      }
      // 内部指针交换
      swap(buffersToWrite, fullBuffers_);
    }
    // 后端写入
    int bufferSize = fullBuffers_.size();
    for (int i = 0; i < bufferSize; ++i) {
      file.write(fullBuffers_[i]->data(), fullBuffers_[i]->length());
      fullBuffers_[i]->clear();
      fullBuffers_[i]->reset();
    }
    // 重新加入到空闲队列中
    {
      lock_guard<mutex> guard(mutex_);
      while (!buffersToWrite.empty()) {
        freeBuffers_.push_back(std::move(buffersToWrite.back()));
        buffersToWrite.pop_back();
      }
    }
  }

}
