#include "../Condition.h"
#include "../Mutex.h"
#include "../Thread.h"
#include "../CurrentThread.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;
using namespace web;

Mutex mutex;
int a = 0;

void threadFunc() {
  {
    LockGuard guard(mutex);
    cout << web::CurrentThread::getName() << '\n';
    a += 1;
  }
  LockGuard guard(mutex);
  cout << web::CurrentThread::getName() << '\n';
  a += 1;
}

int main() {
  vector<unique_ptr<Thread>> threads;
  int threadNum = 10;

  threads.reserve(threadNum);
  for (int i = 0; i < threadNum; ++i) {
    threads.emplace_back(new Thread(threadFunc));
  }

  for (int i = 0; i < threadNum; ++i) {
    threads[i]->start();
    threads[i]->join();
  }

  assert(a == 2 * threadNum);
  printf("Total Num: %d\n", a);
}