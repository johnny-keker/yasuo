#include "alphabet.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#ifdef MUTEX
// define mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#else
// define rwlock
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
#endif

void lock() {
#ifdef MUTEX
    pthread_mutex_lock(&mutex);
#else
    pthread_rwlock_wrlock(&rwlock);
#endif
}

void unlock() {
#ifdef MUTEX
    pthread_mutex_unlock(&mutex);
#else
    pthread_rwlock_unlock(&rwlock);
#endif
}

void* inv_f(void* args) {
  int* interval = (int *)args;
  for (;;) {
    usleep(*interval);
    lock();
    invert_case();
    unlock();
  }
}

void* swp_f(void* args) {
  int* interval = (int *)args;
  for (;;) {
    usleep(*interval);
    lock();
    swap_alphabet();
    unlock();
  }
}

#ifdef RW
void* cnt_f(void* args) {
  int* interval = (int *)args;
  for (;;) {
    usleep(*interval);
    pthread_rwlock_rdlock(&rwlock);
    int upper = count_uppercase();
    printf("\n<...uppercase=%d...>\n", upper);
    pthread_rwlock_unlock(&rwlock);
  }
}
#endif

int main() {
  // thread descriptors init
  pthread_t inv_thread, swp_thread;
  // setting up intervals (hardcoded for now)
  int inv_interval = 100, swp_interval = 200, main_interval = 50;
#ifdef MUTEX
  // initialize mutex
  pthread_mutex_init(&mutex, NULL);
#else
  // initialize rwlock
  pthread_rwlock_init(&rwlock, NULL);
#endif

#ifdef RW
  // count interval (hardcoded for now)
  int cnt_interval = 10;
  pthread_t cnt_thread;
  pthread_create(&cnt_thread, NULL, cnt_f, (void *)&cnt_interval);
#endif

  // THREADS INIT
  pthread_create(&inv_thread, NULL, inv_f, (void *)&inv_interval);
  pthread_create(&swp_thread, NULL, swp_f, (void *)&swp_interval);
  
  for (;;) {
    usleep(main_interval);
    lock();
    printf("\n<... ");
    print_alphabet();
    printf("...>\n");
    unlock();
  }

  return 0;
}
