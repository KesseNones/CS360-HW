//Jesse A. Jones
//17 Oct, 2022
//CS 360
//Assignment V
//V: 1.12

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<errno.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<unistd.h>
#include<sys/wait.h>
#include<time.h>

#define EATING_MEAN 9
#define EATING_STDDEV 3
//Macros used for randomGaussian call for eating time.

#define THINKING_MEAN 11
#define THINKING_STDDEV 7
//Macros used for randomGaussian call for thinking time.

#define NUM_PHILOSOPHERS 5
//Macro used to define how many philosophers are involed in the dining problem.

/* successive calls to randomGaussian produce integer return values */
/* having a gaussian distribution with the given mean and standard  */
/* deviation.  Return values may be negative.                       */

int randomGaussian(int mean, int stddev) {
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

int forkErrCheck(int retVal){
	//This function error checks the return value of a fork function. 

	int errnoTemp;

	if (retVal == -1){
		errnoTemp = errno;
		fprintf(stderr, "%s\n", strerror(errnoTemp));
		return 1;
	}

	return 0;
}

int semopErrCheck(int retVal){
	//This function error checks the return value of a semop function. 

	int errnoTemp;

	if (retVal == -1){
		errnoTemp = errno;
		fprintf(stderr, "%s\n", strerror(errnoTemp));
		return 1;
	}

	return 0;
}

void semSetRemove(int semSetId){
	//This function removes the semephore set based on the ID passed into it.

	int errnoTemp;

	int semctlRet = semctl(semSetId, 0, IPC_RMID);

	//Error checks removal of semaphore set.
	if (semctlRet == -1){
		errnoTemp = errno;
		fprintf(stderr, "%s\n", strerror(errnoTemp));
		exit(errnoTemp);
	}

}

void fallOver(int *pidArr){
	//This function kills all active child processes 
	// during an error based exit of the whole program sequence.

	//Kills all non-zero pids that are active children.
	for (int i = 0; i < NUM_PHILOSOPHERS + (NUM_PHILOSOPHERS == 1); i++){
		if (pidArr[i] > 0){
			kill(pidArr[i], 9);
		}
	}
}

void philoFunc(int philophNumber, int semSetId){
	//This function performs the dining philosophers 
	// and interacts with the semephore set.

	int timeToThink, timeToDine;
	int totalDiningTime = 0;
	int totalThinkingTime = 0;
	int cycleCount = 0;
	int semOpResult;

	pid_t pid = getpid();

	int firstChopIndex, secondChopIndex;

	firstChopIndex = philophNumber;
	secondChopIndex = philophNumber + 1;

	//Accounts for second chop index being past the end of the semaphore set.
	if (secondChopIndex == (NUM_PHILOSOPHERS + (NUM_PHILOSOPHERS == 1))){
		secondChopIndex = 0;
	}

	struct sembuf waitToAccessChopsticks[2] = {{firstChopIndex, -1, 0}, {secondChopIndex, -1, 0}};
	struct sembuf releaseChopsticks[2] = {{firstChopIndex, 1, 0}, {secondChopIndex, 1, 0}};

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
		// for the 0th cycle before beginning to eat.
		if (cycleCount != 0){
		semOpResult = semop(semSetId, waitToAccessChopsticks, 2);
		
		semopErrCheck(semOpResult);

		printf("Philosopher: %d eating for %d seconds using chopsticks %d and %d. (EatTotal = %d)\n", 
			philophNumber, timeToDine, firstChopIndex, secondChopIndex, totalDiningTime);
		sleep(timeToDine);
		totalDiningTime += timeToDine;

		semOpResult = semop(semSetId, releaseChopsticks, 2);

		semopErrCheck(semOpResult);

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

	printf("\nPhilosopher: %d done with meal (process %d)\n", philophNumber, pid);

 	printf("-----Philosopher: %d thought for %d seconds and ate for %d seconds over %d cycles\n\n", 
 		philophNumber, totalThinkingTime, totalDiningTime, cycleCount);

}

int main(int argc, char *argv[]){
	int forkRet;

	int semOpRet;

	int philophNumber;

	int errnoTemp;

	int semSetId = semget(IPC_PRIVATE, (NUM_PHILOSOPHERS + (NUM_PHILOSOPHERS == 1)), IPC_CREAT | IPC_EXCL | 0600);

	//Error checks the formation of the semaphore set.
	if (semSetId == -1){
		errnoTemp = errno;
		fprintf(stderr, "%s\n", strerror(errnoTemp));
		exit(errnoTemp);
	}

	struct sembuf setSemToAccess[1] = {{0, 1, 0}};

	//Makes all semaphores accessable initially to avoid an initial deadlock.
	for (int i = 0; i < (NUM_PHILOSOPHERS + (NUM_PHILOSOPHERS == 1)); i++){
		semOpRet = semop(semSetId, setSemToAccess, 1);
		semOpRet = semopErrCheck(semOpRet);

		//If an error occured with semop, 
		// get rid of the semaphore set and exit.
		if (semOpRet){
			semSetRemove(semSetId);
			exit(1);
		}

		setSemToAccess[0].sem_num++;

	}

	unsigned int seed;

	int *childPidArr = (int *)calloc(NUM_PHILOSOPHERS, sizeof(int));

	//Accounts for calloc failing to work.
	if (childPidArr == NULL){
		fprintf(stderr, "%s\n", "Calloc failed exiting...");
		semSetRemove(semSetId);
		exit(1);
	}

	//Creates child processes representing the philosophers.
	for (int i = 0; i < NUM_PHILOSOPHERS; i++){
		forkRet = fork();

		//Kills child processes, frees the pid array, 
		// removes the semephore set, and exits if fork failed.
		if (forkErrCheck(forkRet)){
			childPidArr[i] = -1;

			fallOver(childPidArr);
			free(childPidArr);
			semSetRemove(semSetId);
			
			exit(1);
		}

		//Adds pid of child process to array if it's valid.
		if (forkRet > 0){
			childPidArr[i] = forkRet;
		}

		//Makes a seed for randomGaussian and calls 
		// the function for the philosopher 
		// to use to think and eat if the process is one of the children.
		if (!forkRet){
			free(childPidArr);

			philophNumber = i;

			seed = time(NULL);

			//Gets pid as alternative seed 
			// if it failed to fetch unix time stamp.
			if (seed == -1){
				seed = getpid();
			}

			srand(seed + philophNumber);

			philoFunc(philophNumber, semSetId);

			break;
		}

	}

	int status;

	int exitStatus;

	int waitStatus;

	//If it's the main process, wait for the children 
	// to finish and perform necessary error checks.
	if (forkRet != 0){
		for (int i = 0; i < NUM_PHILOSOPHERS; i++){
			waitStatus = wait(&status);

			//Error checks wait.
			if(waitStatus == -1){
				errnoTemp = errno;
				fprintf(stderr, "%s\n", strerror(errnoTemp));

				fallOver(childPidArr);
				free(childPidArr);

				semSetRemove(semSetId);

				exit(errnoTemp);
			}

			//Checks for a child process exiting erroniously.
			if (WIFEXITED(status)){
				exitStatus = WEXITSTATUS(status);
				if (exitStatus > 0){
					fallOver(childPidArr);
					free(childPidArr);

					semSetRemove(semSetId);

					exit(1);
				}
			}

		}

		free(childPidArr);
		semSetRemove(semSetId);
		exit(0);

	}

	return 0;
}