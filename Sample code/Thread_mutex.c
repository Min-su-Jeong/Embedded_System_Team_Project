#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int share_var = 1;
pthread_mutex_t mid;

int main() {
	pthread_t ptProduc, ptConsum;
	pthread_mutex_init(&mid, NULL);
	pthread_create(&ptProduc, NULL, producerFunc, NULL);
	pthread_create(&ptConsum, NULL, consumerFunc, NULL);
	pthread_join(ptProduc, NULL);
	pthread_join(ptConsum, NULL);
	pthread_mutex_destroy(&mid);
	return 0;
}

void *producerFunc(void *arg) {
	for(int i = 0; i<5; i++) {
		pthread_mutex_lock(&mid);
		printf("[%d] Producer : %d -> ", i, share_var);
		share_var++;
		printf(" %d \n", share_var);
		pthread_mutex_unlock(&mid);
		sleep(2);
	}
	return NULL;
}

void *consumerFunc(void *arg) {
	for(int i=0; i<5; i++) {
		pthread_mutex_lock(&mid);
		printf("[%d] Consumer : %d -> ", i, share_var);
		share_var--;
		printf(" %d \n", share_var);
		pthread_mutex_unlock(&mid);
		sleep(1);
	}
	return NULL;
}
