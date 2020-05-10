#include "Condition.h"
#include "Mutex.h"
#include <cassert>

using namespace web;

Condition::Condition(Mutex &mutex) : mutex_(mutex) {
  MCHECK(pthread_cond_init(&cond_, nullptr));
}

Condition::~Condition() {
  MCHECK(pthread_cond_destroy(&cond_));
}

void Condition::wait() {
  MCHECK(pthread_cond_wait(&cond_, mutex_.getPthreadMutex()));
}

void Condition::waitIf(const std::function<bool()> &cond) {
  while (cond()) {
    MCHECK(pthread_cond_wait(&cond_, mutex_.getPthreadMutex()));
  }
}

void Condition::notify() {
  MCHECK(pthread_cond_signal(&cond_));
}

void Condition::notifyAll() {
  MCHECK(pthread_cond_broadcast(&cond_));
}