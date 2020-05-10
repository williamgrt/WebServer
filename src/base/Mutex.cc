#include "Mutex.h"
#include <cstdio>

using namespace web;

Mutex::Mutex() {
  int r = pthread_mutex_init(&mutex_, nullptr);
  if (r != 0) {
    perror("create mutex error.");
  }
}

Mutex::~Mutex() {
  int r = pthread_mutex_destroy(&mutex_);
  if (r != 0) {
    perror("mutex destroy error.");
  }
}

void Mutex::lock() {
  int r = pthread_mutex_lock(&mutex_);
  if (r != 0) {
    perror("mutex lock error.");
  }
}

void Mutex::unlock() {
  int r = pthread_mutex_unlock(&mutex_);
  if (r != 0) {
    perror("mutex unlock error.");
  }
}
