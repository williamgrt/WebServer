//
// Created by 郭睿韬 on 2020/3/6.
//

#include "../thread.h"
#include <cstdio>

void threadFunc1() {
    printf("Hello World\n");
}

void threadFunc2() {
  printf("Hello\n");
}

int main() {
    server::Thread thread1(threadFunc1), thread2(threadFunc2);
    thread2.start();
    thread1.start();
    thread2.join();
    thread1.join();
}