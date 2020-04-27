#include "Mutex.h"
#include <cstdio>

using namespace web;

Mutex::Mutex() {
  int r = pthread_mutex_init(&mtx_, nullptr);
  if (r == -1) {
    perror("create mutex error.");
  }
}

Mutex::~Mutex() {
  int r = pthread_mutex_destroy(&mtx_);
  if (r == -1) {
    perror("mutex destroy error.");
  }
}

void Mutex::Lock() {
  int r = pthread_mutex_lock(&mtx_);
  if (r == -1) {
    perror("mutex lock error.");
  }
}

void Mutex::Unlock() {
  int r = pthread_mutex_unlock(&mtx_);
  if (r == -1) {
    perror("mutex unlock error.");
  }
}
