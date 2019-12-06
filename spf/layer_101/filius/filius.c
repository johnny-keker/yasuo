#include "alphabet.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// define mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* inv_f(void* args) {
	int* interval = (int *)args;
	for (;;) {
		usleep(*interval);
    pthread_mutex_lock(&mutex);
		invert_case();
		printf("\n<...inverted-case...>\n");
		print_alphabet();
		printf("\n<...inverted-case...>\n");
    pthread_mutex_unlock(&mutex);
	}
}

void* swp_f(void* args) {
	int* interval = (int *)args;
	for (;;) {
		usleep(*interval);
    pthread_mutex_lock(&mutex);
		swap_alphabet();
		printf("\n<...swap...>\n");
		print_alphabet();
		printf("\n<...swap...>\n");
    pthread_mutex_unlock(&mutex);
	}
}

int main() {
  // thread descriptors init
	pthread_t inv_thread, swp_thread;
  // setting up intervals (hardcoded for now)
	int inv_interval = 100, swp_interval = 200;
	
  // initialize mutex
  pthread_mutex_init(&mutex, NULL);

  // THREADS INIT
	pthread_create(&inv_thread, NULL, inv_f, (void *)&inv_interval);
	pthread_create(&swp_thread, NULL, swp_f, (void *)&swp_interval);
  // THREADS START
	pthread_join(inv_thread, NULL);
	pthread_join(swp_thread, NULL);
	return 0;
}
