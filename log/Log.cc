#include <cstring>
#include "Log.h"
#include "Singleton.h"
#include "Params.h"

using namespace gweb;
using namespace std;

const char *LOG_NAME = "log.txt";

Log::Log() :
    ringBuffer_(Params::BUF_LEN, string()),
    currSec_(Time::nowSec()),
    stop_(false),
    lastRead_(-1L),
    lastWrote_(-1L),
    next_(0) {
  // 打开对应的写入文件
  file_ = fopen(LOG_NAME, "w");
  // 创建后端写入线程
  backThread_ = make_unique<thread>([this] {
    this->flushToDisk();
  });
}

Log::~Log() {
  stop_.store(true);
  condVar_.notify_one();
  backThread_->join();
  // 等待后台刷新县城执行完毕退出后再关闭
  fclose(file_);
}

void Log::write(Level level, int val) {
  write(levelString[level] + to_string(val));
}

void Log::write(Level level, unsigned int val) {
  write(levelString[level] + to_string(val));
}

void Log::write(Level level, const char *log) {
  write(levelString[level] + log);
}

void Log::write(Level level, char c) {
  write(levelString[level] + c);
}

void Log::write(Level level, const string &log) {
  write(levelString[level] + log);
}

void Log::write(const string &log) {
  if (Time::nowSec() > currSec_) {
    // 更新缓存时间
    // TODO: add code for update time
    updateTime();
  }
  // 构造写入的日志行
  string writeLog(log.size() + timeStr_.size() + 2, ' '); // 实际写入的日志大小
  memcpy(&*writeLog.begin(), timeStr_.data(), timeStr_.size());
  writeLog[timeStr_.size()] = ' ';
  memcpy(&*(writeLog.begin() + timeStr_.size() + 1), log.data(), log.size());
  writeLog.back() = '\n';
  // 找到写入的位置
  auto currWrite = next_.fetch_add(1);
  ringBuffer_[currWrite & (Params::BUF_LEN - 1)] = std::move(writeLog);
  // 等待前面的日志写入到内部
  while (currWrite - 1L != lastWrote_.load()) {}
  // 正式更改写入位置
  lastWrote_.store(currWrite);
  if (currWrite == lastRead_.load() + 1) {
    condVar_.notify_one();
  }
}

void Log::updateTime() {
  currSec_ = Time::now();
  struct tm *tm;
  // TODO: cache current time
}

void Log::flushToDisk() {
  while (true) {
    // 此时整个系统已经停下来了，并且日志都已经刷新完毕了，直接退出
    if (stop_.load() && lastWrote_.load() == lastRead_.load()) {
      return;
    }
    while (lastRead_.load() < lastWrote_.load()) {
      // 还有日志没有写完
      while (lastRead_.load() < lastWrote_.load()) {
        int64_t currRead = lastRead_.fetch_add(1);
        string data = ringBuffer_[currRead & (Params::BUF_LEN - 1)];
        fwrite(data.data(), data.size(), 1, file_);
        data.clear();
      }
      // 写完了所有的日志，刷新缓冲区
      fflush(file_);
      // 所有的日志都写完了，写线程等待被唤醒
      {
        unique_lock<mutex> lock(mutex_);
        while (!stop_.load() && lastRead_.load() == lastWrote_.load()) {
          condVar_.wait(lock);
        }
      }
    }
  }
}