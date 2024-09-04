#include<pthread.h>
#include<stdio.h>
#include<errno.h>

int glob;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


static void *foo(void *arg){
	int loop = *(int *)arg;
	for (int i = 0; i < loop; ++i){
		glob++;
	}
}

static void *foo2(void *arg){
	int loop = *(int *)arg;
	for (int i = 0; i < loop; ++i){
		pthread_mutex_lock(&mtx);
		glob++;
		pthread_mutex_unlock(&mtx);
	}
}

static void *foo3(void *arg){
	int loop = *(int *)arg;
	for (int i = 0; i < loop; ++i){
		if (pthread_mutex_trylock(&mtx) == EBUSY){
			i--;
			printf("Busy\n");
		}else{
			glob++;
			pthread_mutex_unlock(&mtx);
		}
	}
}

int main(int argc, char const *argv[]){
	int loop = 1000000;

	pthread_t thread1, thread2, thread3;

	pthread_create(&thread1, NULL, foo3, &loop);
	pthread_create(&thread2, NULL, foo3, &loop);
	pthread_create(&thread3, NULL, foo3, &loop);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	printf("%d\n", glob); 


	return 0;
}