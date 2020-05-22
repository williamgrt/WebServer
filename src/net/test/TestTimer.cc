#include "../EventLoop.h"
#include "../Timer.h"
#include "../../base/Thread.h"
#include <cstdio>

using namespace web;
using namespace std;

EventLoop *g_loop;
TimerId tid {};

void func1() {
  printf("call func1!\n");
}

void func2() {
  printf("call func2!\n");
}

void func3() {
  printf("call func3!\n");
  g_loop->cancel(tid);
}

void func4() {
  printf("loop quit!\n");
  g_loop->quit();
}

void threadFunc() {
  g_loop->runAfter(2, func1);
  tid = g_loop->runEvery(3, func2);
  g_loop->runAfter(25, func3);
  g_loop->runAfter(30, func4);
}

int main() {
  EventLoop loop;
  g_loop = &loop;
  Thread t(threadFunc);
  t.start();
  g_loop->loop();
}