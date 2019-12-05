#include "alphabet.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* inv_f(void* args) {
	int* interval = (int *)args;
	for (;;) {
		sleep(*interval);
		invert_case();
		printf("\n<...inverted-case...>\n");
		print_alphabet();
		printf("\n<...inverted-case...>\n");
	}
}

int main() {
	pthread_t inv_thread;
	int inv_interval = 1;
	
	pthread_create(&inv_thread, NULL, inv_f, (void *)&inv_interval);
	pthread_join(inv_thread, NULL);

	return 0;
}
