//
// Created by 郭睿韬 on 2020/3/7.
//

#include "../thread.h"
#include "../mutex.h"
#include "../condvar.h"


server::Mutex mtx;
server::CondVar condVar(mtx);

int avail = 0;
const int full = 3;
/*
pthread_mutex_t mutex_ = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
*/

void Produce() {
  bool done = false;
  int numProduced = 0;
  for (;;) {
    mtx.lock();
    //pthread_mutex_lock(&mutex_);
    avail++;
    numProduced++;
    printf("Producer produces 1, now available num is: %d\n", avail);
    //pthread_mutex_unlock(&mutex_);
    mtx.unlock();
    condVar.notify();
    done = numProduced >= full;
    //pthread_cond_signal(&cond);

    if (done) {
      break;
    }
  }
}

void Consume() {
  bool done = false;
  int numConsumed = 0;

  for (;;) {
    mtx.lock();
    //pthread_mutex_lock(&mutex_);
    while (avail == 0) {
      //pthread_cond_wait(&cond, &mutex_);
      condVar.wait();
    }
    while (avail > 0) {
      --avail;
      printf("Consumer consumes 1, now available num is: %d\n", avail);
      ++numConsumed;
      done = numConsumed >= full;
    }
    mtx.unlock();
    //pthread_mutex_unlock(&mutex_);

    if (done) {
      break;
    }
  }
}

int main() {

}