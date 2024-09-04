//Jesse A. Jones
//25 Oct, 2022
//CS 360
//Assignment VI
//V: 1.11

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<errno.h>
#include<string.h>
#include<pthread.h>
#include<time.h>
#include<sys/types.h>
#include<unistd.h>

//Macros used for randomGaussian call for eating time.
#define EATING_MEAN 9
#define EATING_STDDEV 3

//Macros used for randomGaussian call for thinking time.
#define THINKING_MEAN 11
#define THINKING_STDDEV 7

//Macro used to define how many philosophers are involed in the dining problem.
#define NUM_PHILOSOPHERS 5

//Creates array of mutex representing the chopsticks.
static pthread_mutex_t mtxArr[NUM_PHILOSOPHERS + (NUM_PHILOSOPHERS == 1)];

int mutRet;

/* successive calls to randomGaussian produce integer return values */
/* having a gaussian distribution with the given mean and standard  */
/* deviation.  Return values may be negative.                       */
int randomGaussian(int mean, int stddev){
	//Function taken from random.c file to make this program work.
	// Jesse did not implement this. 

	double mu = 0.5 + (double) mean;
	double sigma = fabs((double) stddev);
	double f1 = sqrt(-2.0 * log((double) rand() / (double) RAND_MAX));
	double f2 = 2.0 * 3.14159265359 * (double) rand() / (double) RAND_MAX;
	if (rand() & (1 << 5)) 
		return (int) floor(mu + sigma * cos(f2) * f1);
	else            
		return (int) floor(mu + sigma * sin(f2) * f1);
}

static void *philoFunc(void *arg){
	//This function makes a given philosopher 
	// eat and think via mutex and threading.

	//Gets philosopher number from arg via casting and pointer mechanics.
	int philophNumber = *(int *)arg;

	int timeToThink, timeToDine;
	int totalDiningTime = 0;
	int totalThinkingTime = 0;
	int cycleCount = 0;
	int firstChopIndex, secondChopIndex;
	int chop1Ret, chop2Ret;

	unsigned int seed;

	seed = time(NULL);

	//Gets pid as alternative seed 
	// if it failed to fetch unix time stamp.
	if (seed == -1){
		seed = getpid();
	}

	//Sets random seed for given philosopher thread.
	srand(seed + philophNumber);

	firstChopIndex = philophNumber;
	secondChopIndex = philophNumber + 1;

	//Accounts for second chop index being past the end of the mutex array.
	if (secondChopIndex == (NUM_PHILOSOPHERS + (NUM_PHILOSOPHERS == 1))){
		secondChopIndex = 0;
	}

	//This loop makes the given philosopher eat and think 
	// until 100 seconds worth of eating time has occured.
	while(totalDiningTime < 100){
		timeToThink = randomGaussian(THINKING_MEAN, THINKING_STDDEV);
		timeToDine = randomGaussian(EATING_MEAN, EATING_STDDEV);

		if (timeToThink < 0){
			timeToThink = 0;
		}
		if (timeToDine < 0){
			timeToDine = 0;
		}

		//This if statement prevents the philosophers 
		// from eating for one round, making them all think 
		// for the 0th cycle before beginning to eat in the next cycle.
		if (cycleCount != 0){

			//Waits for both chopsticks to be available, 
			// then grabs them when they are.
			do {
				chop1Ret = pthread_mutex_trylock(&(mtxArr[firstChopIndex]));
				chop2Ret = pthread_mutex_trylock(&(mtxArr[secondChopIndex]));

				//If first chopstick is occupied but second is available, 
				// drop second chopstick. 
				if (chop1Ret == EBUSY && chop2Ret == 0){
					pthread_mutex_unlock(&(mtxArr[secondChopIndex]));
				}
				
				//If first chopstick is available 
				// and second chopstick is taken, drop first chopstick.
				if(chop1Ret == 0 && chop2Ret == EBUSY){
					pthread_mutex_unlock(&(mtxArr[firstChopIndex]));
				}

			}while((chop1Ret == EBUSY) || (chop2Ret == EBUSY));

			printf("Philosopher: %d eating for %d seconds using chopsticks %d and %d. (EatTotal = %d)\n", 
				philophNumber, timeToDine, firstChopIndex, secondChopIndex, totalDiningTime);
			sleep(timeToDine);
			totalDiningTime += timeToDine;

			//Releases chopsticks after eating.
			pthread_mutex_unlock(&(mtxArr[firstChopIndex]));
			pthread_mutex_unlock(&(mtxArr[secondChopIndex]));

			printf("\tPhilosopher: %d finsihed eating for %d seconds with chopsticks %d and %d. (EatTotal = %d)\n", 
				philophNumber, timeToDine, firstChopIndex, secondChopIndex, totalDiningTime);

		}

		printf("Philosopher: %d thinking for %d seconds. (ThinkTotal = %d)\n", 
			philophNumber, timeToThink, totalThinkingTime);
		sleep(timeToThink);
		totalThinkingTime += timeToThink;

		printf("\tPhilosopher: %d finished thinking for %d seconds. (ThinkTotal = %d)\n", 
			philophNumber, timeToThink, totalThinkingTime);
		
		cycleCount++;

	}

 	printf("\n[[[[[[[[[Philosopher: %d thought for %d seconds and ate for %d seconds over %d cycles]]]]]]]]]\n\n", 
 		philophNumber, totalThinkingTime, totalDiningTime, cycleCount);

 	return NULL;

}

int main(int argc, char *argv[]){
	int createThreadRetVal,threadJoinRetVal;

	int errnoTemp;

	mutRet = pthread_mutex_init(mtxArr, NULL);

	//Error checks creation of mutex array.
	if (mutRet != 0){
		errnoTemp = mutRet;
		fprintf(stderr, "%s\n", strerror(errnoTemp));
		exit(errnoTemp);
	}

	pthread_t *pthreadArr = (pthread_t *)malloc(NUM_PHILOSOPHERS * sizeof(pthread_t));

	int *philophNumberArr = (int *)malloc(NUM_PHILOSOPHERS * sizeof(int));

	//Error checks malloc.
	if (pthreadArr == NULL){
		fprintf(stderr, "%s\n", "Malloc failed exiting...");
		free(pthreadArr);
		free(philophNumberArr);
		return 1;
	}

	//Creates n threads representing the n philosophers.
	for (int i = 0; i < NUM_PHILOSOPHERS; i++){
		philophNumberArr[i] = i;

		createThreadRetVal = pthread_create(&pthreadArr[i], NULL, philoFunc, &philophNumberArr[i]);

		//Error checks creation of thread.
		if (createThreadRetVal != 0){
			errnoTemp = createThreadRetVal;
			fprintf(stderr, "%s\n", strerror(errnoTemp));
			free(pthreadArr);
			free(philophNumberArr);
			return 2;
		}
	}

	//Waits for each philosopher to finish.
	for (int i = 0; i < NUM_PHILOSOPHERS; i++){
		threadJoinRetVal = pthread_join(pthreadArr[i], NULL);

		//Error checks waiting for philosophers.
		if (threadJoinRetVal != 0){
			errnoTemp = threadJoinRetVal;
			fprintf(stderr, "%s\n", strerror(errnoTemp));
			free(pthreadArr);
			free(philophNumberArr);
			return 3;
		}
	}

	free(pthreadArr);
	free(philophNumberArr);

	return 0;
}
