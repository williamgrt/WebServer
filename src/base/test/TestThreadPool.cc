//
// Created by 郭睿韬 on 2020/5/8.
//
#include <iostream>
#include "../ThreadPool.h"
#include "../CurrentThread.h"

using namespace std;
using namespace web;

void print() {
  cout << CurrentThread::getName() << '\n';
}

void test(int maxSize) {
  int poolSize = 10;
  ThreadPool pool(maxSize, poolSize);
  pool.start();
  for (int i = 0; i < 100; ++i) {
    pool.addTask(print);
  }
  pool.stop();
}

int main() {
  //test(0);
  // test(1);
  test(10);
  return 0;
}
