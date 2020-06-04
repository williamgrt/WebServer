#include "Logger.h"
#include "CurrentThread.h"
#include "MmapFile.h"
#include <vector>
#include <cassert>

using namespace web;
using namespace std;

Logger::Logger(const std::string &fileName)
    : logName_(fileName),
      buffers_(),
      bufferMutex_(),
      freeBufferQueue_(),
      freeMutex_(),
      flushBufferQueue_(),
      flushMutex_(),
      flushCond_(flushMutex_),
      flushThread_(std::bind(&Logger::flush, this))
      {
}

void Logger::start() {
  running_ = true;
  // 开启后台刷新线程
  flushThread_.start();
}

void Logger::append(const char *line, int len) {
  // 当前线程已经提前缓存
  Thread::id currTid = web::CurrentThread::getId();
  BufferPtr writeBuffer;
  // 获取当前线程的缓冲区
  {
    LockGuard guard(bufferMutex_);
    auto ptr = buffers_.find(currTid);
    if (ptr == buffers_.end()) {
      buffers_[currTid] = make_unique<Buffer>();
    }
    writeBuffer = buffers_[currTid];
  }

  // 如果当前缓冲区没有多余的空间
  if (writeBuffer->available() < len) {

    // 寻找新的空闲缓冲区
    BufferPtr newBuffer;
    {
      // 访问空闲队列寻找可用缓存
      LockGuard guard(freeMutex_);
      if (!freeBufferQueue_.empty()) {
        // 目前在空闲队列中存在没有使用的缓冲区
        newBuffer = freeBufferQueue_.front();
        freeBufferQueue_.pop_front();
      } else {
        // 向操作系统申请一块新的缓冲区
        newBuffer = std::make_shared<Buffer>();
      }
    }

    // 把旧的缓冲区写入到刷新队列
    {
      LockGuard guard(flushMutex_);
      flushBufferQueue_.push_back(writeBuffer);
      // 通知后台线程可以进行刷新操作
      flushCond_.notify();
    }

    writeBuffer = newBuffer; // 更改当前线程
    // 更新线程对应的缓冲区
    {
      LockGuard guard(bufferMutex_);
      buffers_[currTid] = writeBuffer;
    }
  }

  // 把日志写入缓冲区
  writeBuffer->append(line, len);
}

void Logger::stop() {
  running_ = false;

  // 唤醒后端线程
  flushCond_.notify();
}

void Logger::flush() {
  assert(running_);
  // 打开对应的日志刷新文件
  MmapFile file_;
  file_.open(logName_);

  // 一直循环监听，知道日志器被关闭
  while (running_) {
    std::vector<BufferPtr> tmpBuffer;
    // 检查刷新队列中是否存在需要刷新的缓冲区日志
    // 如果有，弹出所有的缓冲区日志
    {
      LockGuard guard(flushMutex_);
      while (flushBufferQueue_.empty() && running_) {
        // 让后端刷新线程等待3秒，然后再判断是否需要刷新日志
        flushCond_.waitForSeconds(3);
      }

      if (!running_) {
        // 异步日志已经关闭了
        return;
      } else {
        tmpBuffer.assign(flushBufferQueue_.begin(), flushBufferQueue_.end());
        // 清空缓存队列
        flushBufferQueue_.clear();
      }
    }

    // 如果有需要写入磁盘的缓冲区
    // 执行写入操作，并清空缓冲区
    if (!tmpBuffer.empty()) {
      for (auto &buffer: tmpBuffer) {
        const char *data = buffer->data();
        file_.write(data, buffer->length());
        buffer->clear();
        buffer->reset();
      }
      // file_.sync();
      // 重新添加到空闲队列中
      {
        LockGuard guard(freeMutex_);
        freeBufferQueue_.insert(freeBufferQueue_.end(), tmpBuffer.begin(), tmpBuffer.end());
      }
    }
  }
}
