#include "../Thread.h"
#include <functional>
#include <iostream>
#include <string>

using namespace web;
using namespace std;

void threadFunc() { cout << "hello world\n"; }

int main() {
  Thread t1(threadFunc, ""), t2(threadFunc, "");
  t1.start();
  t1.join();
  t2.start();
  t2.join();
  return 0;
}
