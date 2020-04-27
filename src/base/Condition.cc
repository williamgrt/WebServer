#include "Condition.h"
#include "Mutex.h"
#include <cassert>

using namespace web;

Condition::Condition(Mutex &mutex) : mutex_(mutex) {
  MCHECK(pthread_cond_init(&cond_, nullptr));
}

Condition::~Condition() { MCHECK(pthread_cond_destroy(&cond_)); }

void Condition::Wait() {
  MCHECK(pthread_cond_wait(&cond_, mutex_.GetPthreadMutex()));
}

void Condition::WaitIf(std::function<bool()> cond) {
  while (!cond()) {
    MCHECK(pthread_cond_wait(&cond_, mutex_.GetPthreadMutex()));
  }
}

void Condition::Notify() { MCHECK(pthread_cond_signal(&cond_)); }

void Condition::NotifyAll() { MCHECK(pthread_cond_broadcast(&cond_)); }