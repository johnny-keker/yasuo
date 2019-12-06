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
    printf("\n<...inverted-case...>\n");
    print_alphabet();
    printf("\n<...inverted-case...>\n");
    unlock();
  }
}

void* swp_f(void* args) {
  int* interval = (int *)args;
  for (;;) {
    usleep(*interval);
    lock();
    swap_alphabet();
    printf("\n<...swap...>\n");
    print_alphabet();
    printf("\n<...swap...>\n");
    unlock();
  }
}

int main() {
  // thread descriptors init
  pthread_t inv_thread, swp_thread;
  // setting up intervals (hardcoded for now)
  int inv_interval = 100, swp_interval = 200;
#ifdef MUTEX
  // initialize mutex
  pthread_mutex_init(&mutex, NULL);
#else
  // initialize rwlock
  pthread_rwlock_init(&rwlock, NULL);
#endif
  // THREADS INIT
  pthread_create(&inv_thread, NULL, inv_f, (void *)&inv_interval);
  pthread_create(&swp_thread, NULL, swp_f, (void *)&swp_interval);
  // THREADS START
  pthread_join(inv_thread, NULL);
  pthread_join(swp_thread, NULL);
  return 0;
}
