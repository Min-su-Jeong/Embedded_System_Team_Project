#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *threadFunc (void *data){
	int pram = *((int *) data);
	int i = 0;
	while(1) {
		printf("thread ID: %lu, Parameter: %d, Iter: %d\n", pthread_self(), pram, i);
		i++;
		if(i==10) return (void*)i;
		sleep(1);
	}
}

int main(void) {
	pthread_t p_thread;
	int r_val;
	int status;
	int value = 229;
	
	r_val = pthread_create(&p_thread, NULL, threadFunc, (void*)&value);
	if(r_val < 0) {
		perror("pthread_create() error\n");
		exit(0);
	}
	
	r_val = pthread_join(p_thread, (void**)&status);
	if(r_val < 0) {
		perror("pthread_join() error\n");
		exit(0);
	}
	printf("Return value: %d\n", status);
	return 0;
}
