#include "alphabet.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* inv_f(void* args) {
	int* interval = (int *)args;
	for (;;) {
		usleep(*interval);
		invert_case();
		printf("\n<...inverted-case...>\n");
		print_alphabet();
		printf("\n<...inverted-case...>\n");
	}
}

void* swp_f(void* args) {
	int* interval = (int *)args;
	for (;;) {
		usleep(*interval);
		swap_alphabet();
		printf("\n<...swap...>\n");
		print_alphabet();
		printf("\n<...swap...>\n");
	}
}

int main() {
  // thread descriptors init
	pthread_t inv_thread, swp_thread;
  // setting up intervals (hardcoded for now)
	int inv_interval = 100, swp_interval = 200;
	
  // THREADS INIT
	pthread_create(&inv_thread, NULL, inv_f, (void *)&inv_interval);
	pthread_create(&swp_thread, NULL, swp_f, (void *)&swp_interval);
  // THREADS START
	pthread_join(inv_thread, NULL);
	pthread_join(swp_thread, NULL);
	return 0;
}
