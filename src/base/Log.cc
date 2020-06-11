#include "Log.h"
#include "Logger.h"
#include <pthread.h>
#include <chrono>

using namespace web;
using namespace std;

static pthread_once_t attr_ = PTHREAD_ONCE_INIT;
static unique_ptr<Logger> logger_;
string Log::logFileName_ = "./WebServer.txt";

// TODO: 改为多线程的单例模式
void init() {
  // 创建后台的日志写入线程，并启动
  logger_ = make_unique<Logger>(Log::getLogFileName());
  logger_->start();
}

void output(const char *line, size_t len) {
  pthread_once(&attr_, init);
  logger_->append(line, len);
}

Log::Log(const char *name, int line) :
    baseName_(name), line_(line), level_(LogLevel::TRACE) {
  formatTime();
}

// 在析构函数中把日志写入到后台线程中
Log::~Log() {
  stream_ << "--" << baseName_ << ":" << line_ << "\n";
  const LogBuffer<kSmallSize> &buffer = stream_.buffer();
  output(buffer.data(), buffer.length());
}

void Log::formatTime() {
  using chrono::system_clock;
  // 获取当前时间
  system_clock::time_point now = system_clock::now();
  std::time_t tt = system_clock::to_time_t(now);
  // 把时间写入到字符串中
  char str[26]{};
  auto localTime = localtime(&tt);
  strftime(str, 26, "%Y-%m-%d %H:%M:%S\n", localTime);
  stream_ << str;
}