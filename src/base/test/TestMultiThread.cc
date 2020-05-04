#include "../Condition.h"
#include "../Mutex.h"
#include "../Thread.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace web;

Mutex mutex;
int a = 0;

void threadFunc() {
  LockGuard guard(mutex);
  a += 1;
}

int main() {
  Thread t1(threadFunc, ""), t2(threadFunc, "");
  t1.Start();
  t1.Join();
  t2.Start();
  t2.Join();
  cout << a << '\n';
  assert(a == 2);
  return 0;
}