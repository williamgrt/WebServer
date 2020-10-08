#include "../Log.h"
#include "../Thread.h"
#include <iostream>
#include <sys/time.h>
#include <vector>

using namespace std;
using namespace web;

int64_t get_current_millis() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int64_t) tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// 功能测试
void function_test() {
  cout << "----------type test-----------" << endl;
  LOG_INFO << 1;
  LOG_INFO << 1234567890123;
  LOG_INFO << 1.0f;
  LOG_INFO << 3.1415926;
  LOG_INFO << (short) 1;
  LOG_INFO << (long long) 1;
  LOG_INFO << (unsigned int) 1;
  LOG_INFO << (unsigned long) 1;
  LOG_INFO << (long double) 1.6555556;
  LOG_INFO << (unsigned long long) 1;
  LOG_INFO << 'c';
  LOG_INFO << "abcdefg";
  LOG_INFO << string("This is a string");
}

// 单线程写入测试
void single_thread_test() {
  int logs = 100 * 10000;
  cout << "----------single thread test-----------" << endl;
  uint64_t start_ts = get_current_millis();
  for (int i = 0; i < 1000000; ++i) {
    LOG_INFO << i;
  }
  uint64_t end_ts = get_current_millis();
  printf("1 million times logtest, time use %lums, %ldw logs/second\n",
         end_ts - start_ts,
         100 * 1000 / (end_ts - start_ts));
}

void threadFunc() {
  for (int i = 0; i < 1000000; ++i) {
    LOG_INFO << i;
  }
}

int threadnum = 2;
// 多线程写入测试
void multi_thread_test() {
  printf("multi_thread_test, thread num: %d ...\n", threadnum);
  vector<std::unique_ptr<Thread>> threads;
  for (int i = 0; i < threadnum; ++i) {
    threads.push_back(make_unique<Thread>(threadFunc));
  }
  uint64_t start_ts = get_current_millis();

  for (int i = 0; i < threadnum; ++i) {
    threads[i]->start();
    threads[i]->join();
  }

  uint64_t end_ts = get_current_millis();
  printf("%d million times logtest, time use %lums, %ldw logs/second\n",
         threadnum,
         end_ts - start_ts,
         threadnum * 1000000 / (end_ts - start_ts) / 10);
}

int main() {
  function_test();
  single_thread_test();
  multi_thread_test();
  return 0;
}