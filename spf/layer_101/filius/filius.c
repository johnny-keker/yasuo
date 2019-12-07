#include "alphabet.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#if defined SEM_POS
#include <semaphore.h>
#elif defined SEM_SYSV
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#endif



#ifdef MUTEX
// define mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#elif defined RW
// define rwlock
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
#elif defined SEM_POS
// define posix semaphore
sem_t sem;
#else
// define sysv semaphore
int sem_id;
#endif

void lock() {
#ifdef MUTEX
    pthread_mutex_lock(&mutex);
#elif defined RW
    pthread_rwlock_wrlock(&rwlock);
#elif defined SEM_POS
    sem_wait(&sem);
#else
  struct sembuf operations[2] = {
    { .sem_op = 0 }, { .sem_op = 1 }
  };
  semop(sem_id, operations, 2);
#endif
}

void unlock() {
#ifdef MUTEX
    pthread_mutex_unlock(&mutex);
#elif defined RW
    pthread_rwlock_unlock(&rwlock);
#elif defined SEM_POS
    sem_post(&sem);
#else
  struct sembuf operations = { .sem_op = -1 };
  semop(sem_id, &operations, 1);
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

bool parse_int(const char* str, int* var) {
  char* end;
  *var = (int)strtol(str, &end, 10);
  if (end == str) return false;
  return true;
}

int main(int argc, char* argv[]) {
  // thread descriptors init
  pthread_t inv_thread, swp_thread;
  // setting up intervals (hardcoded for now)
  int inv_interval = 100, swp_interval = 200, main_interval = 1000, cnt_interval = 10;
  int opt = 0;
  while ((opt = getopt(argc, argv, "i:s:m:c:")) != -1) {
    switch (opt) {
      case 'i':
        if (!parse_int(optarg, &inv_interval)) {
          printf("-i parameter must be integter!");
          return 1;
        }
        break;
      case 's':
        if (!parse_int(optarg, &swp_interval)) {
          printf("-s parameter must be integter!");
          return 1;
        }
        break;
      case 'm':
        if (!parse_int(optarg, &main_interval)) {
          printf("-m parameter must be integter!");
          return 1;
        }
        break;
      case 'c':
        if (!parse_int(optarg, &cnt_interval)) {
          printf("-c parameter must be integter!");
          return 1;
        }
        break;
      default:
        fprintf(stderr, "Usage: %s [-i] [-s] [-m] [-c]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
#ifdef MUTEX
  // initialize mutex
  pthread_mutex_init(&mutex, NULL);
#elif defined RW
  // initialize rwlock
  pthread_rwlock_init(&rwlock, NULL);
#elif defined SEM_POS
  // initialize posix semaphore
  sem_init(&sem, 0, 1);
#else
  // initialize sysv semaphore
  sem_id = semget(IPC_PRIVATE, 2, 0600 | IPC_CREAT);
  semctl(sem_id, 0, SETALL, 1);
#endif

#ifdef RW
  // count interval (hardcoded for now)
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
