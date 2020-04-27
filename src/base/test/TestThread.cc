#include "../Thread.h"
#include <functional>
#include <iostream>
#include <string>

using namespace web;
using namespace std;

void threadFunc() { cout << "hello world\n"; }

int main() {
  Thread t1(threadFunc, ""), t2(threadFunc, "");
  t1.Start();
  t1.Join();
  t2.Start();
  t2.Join();
  return 0;
}
