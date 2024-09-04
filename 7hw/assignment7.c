//Jesse A. Jones
//2 Nov, 2022
//CS 360
//Assignment VII
//V: 1.05

#include"assignment7.h"
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<stdio.h>
#include<errno.h>

#define SORT_THRESHOLD      40

//Arbitrary lowerbound where threads become 
// too expensive to use on a partition. 
#define PARTITION_LOWERBOUND 2048

typedef struct _sortParams {
    char** array;
    int left;
    int right;
} SortParams;

static int maximumThreads;              /* maximum # of threads to be used */

static int existingThreads;

static pthread_mutex_t countChangeMutex;

static pthread_t *threadArr;

/* This is an implementation of insert sort, which although it is */
/* n-squared, is faster at sorting short lists than quick sort,   */
/* due to its lack of recursive procedure call overhead.          */

static void insertSort(char** array, int left, int right) {
    int i, j;
    for (i = left + 1; i <= right; i++) {
        char* pivot = array[i];
        j = i - 1;
        while (j >= left && (strcmp(array[j],pivot) > 0)) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = pivot;
    }
}

/* Recursive quick sort, but with a provision to use */
/* insert sort when the range gets small.            */

static void *quickSort(void* p) {
    //This function performs quicksort on an array of strings.
    // Threads are employed to increase speed.
    // A large bulk of this code was not implemented by Jesse.

    SortParams* params = (SortParams*) p;
    char** array = params->array;
    int left = params->left;
    int right = params->right;
    int i = left, j = right;

    int errnoTemp, tCreateRes;

    int threadIndex;
    int threadOneCreated = 0;

    int joinFirstRet;
    
    if (j - i > SORT_THRESHOLD){           /* if the sort range is substantial, use quick sort */
    
        int m = (i + j) >> 1;               /* pick pivot as median of         */
        char* temp, *pivot;                 /* first, last and middle elements */
        if (strcmp(array[i],array[m]) > 0) {
            temp = array[i]; array[i] = array[m]; array[m] = temp;
        }
        if (strcmp(array[m],array[j]) > 0) {
            temp = array[m]; array[m] = array[j]; array[j] = temp;
            if (strcmp(array[i],array[m]) > 0) {
                temp = array[i]; array[i] = array[m]; array[m] = temp;
            }
        }
        pivot = array[m];

        for (;;) {
            while (strcmp(array[i],pivot) < 0) i++; /* move i down to first element greater than or equal to pivot */
            while (strcmp(array[j],pivot) > 0) j--; /* move j up to first element less than or equal to pivot      */
            if (i < j) {
                char* temp = array[i];      /* if i and j have not passed each other */
                array[i++] = array[j];      /* swap their respective elements and    */
                array[j--] = temp;          /* advance both i and j                  */
            } else if (i == j) {
                i++; j--;
            } else break;                   /* if i > j, this partitioning is done  */
        }
        
        SortParams first;  first.array = array; first.left = left; first.right = j;

        SortParams second; second.array = array; second.left = i; second.right = right;
    
        //Sets important thread values as well 
        // as incrementing the total thread number.
        // Mutex ensures atomic operation of incrementation.
        pthread_mutex_lock(&countChangeMutex);
        existingThreads++;
        threadIndex = existingThreads - 1;
        pthread_mutex_unlock(&countChangeMutex);

        //Makes recursive call or creates thread 
	// if still available and not too expensive.
        if (((threadIndex + 1) > maximumThreads) || 
			((first.right - first.left) < PARTITION_LOWERBOUND)){
            quickSort(&first);
        }else{

            tCreateRes = pthread_create(&threadArr[threadIndex], NULL, quickSort, &first);

            //Error checks creation of new thread.
            if (tCreateRes != 0){
                errnoTemp = tCreateRes;
                fprintf(stderr, "%d: %s\n", errnoTemp, strerror(errnoTemp));
                exit(errnoTemp);
            }

            threadOneCreated = 1;
        }

        //Sorts right partition.
        quickSort(&second);

        //Waits for given thread.
        if (threadOneCreated){
            joinFirstRet = pthread_join(threadArr[threadIndex], NULL);

            //Error checks waiting for thread.
            if (joinFirstRet != 0){
                errnoTemp = joinFirstRet;
                fprintf(stderr, "ERROR %d: %s\n", errnoTemp, strerror(errnoTemp));
                free(threadArr);
                exit(errnoTemp);
            }

            //Updates count of current active threads 
            // when given thread finishes.
            //pthread_mutex_lock(&countChangeMutex);
            //existingThreads--;
            //pthread_mutex_unlock(&countChangeMutex);
        }


    } else insertSort(array,i,j);           /* for a small range use insert sort */

    return NULL;
}

static void threadlessQuicksort(void* p) {
    //This function is a copy of the original 
    // quicksort implementation found in this file. This function will 
    // be used in comparison with the threaded version of quicksort based 
    // on the modified version of this function located above.

    SortParams* params = (SortParams*) p;
    char** array = params->array;
    int left = params->left;
    int right = params->right;
    int i = left, j = right;
    
    if (j - i > SORT_THRESHOLD) {           /* if the sort range is substantial, use quick sort */

        int m = (i + j) >> 1;               /* pick pivot as median of         */
        char* temp, *pivot;                 /* first, last and middle elements */
        if (strcmp(array[i],array[m]) > 0) {
            temp = array[i]; array[i] = array[m]; array[m] = temp;
        }
        if (strcmp(array[m],array[j]) > 0) {
            temp = array[m]; array[m] = array[j]; array[j] = temp;
            if (strcmp(array[i],array[m]) > 0) {
                temp = array[i]; array[i] = array[m]; array[m] = temp;
            }
        }
        pivot = array[m];

        for (;;) {
            while (strcmp(array[i],pivot) < 0) i++; /* move i down to first element greater than or equal to pivot */
            while (strcmp(array[j],pivot) > 0) j--; /* move j up to first element less than or equal to pivot      */
            if (i < j) {
                char* temp = array[i];      /* if i and j have not passed each other */
                array[i++] = array[j];      /* swap their respective elements and    */
                array[j--] = temp;          /* advance both i and j                  */
            } else if (i == j) {
                i++; j--;
            } else break;                   /* if i > j, this partitioning is done  */
        }
        
        SortParams first;  first.array = array; first.left = left; first.right = j;
        threadlessQuicksort(&first);                  /* sort the left partition  */
        
        SortParams second; second.array = array; second.left = i; second.right = right;
        threadlessQuicksort(&second);                 /* sort the right partition */
                
    } else insertSort(array,i,j);           /* for a small range use insert sort */
}

/* user interface routine to set the number of threads sortT is permitted to use */

void setSortThreads(int count) {
    //Sets the maximum number of threads allowed 
    // when using the multithreaded quicksort.
    maximumThreads = count;
}

/* user callable sort procedure, sorts array of count strings, beginning at address array */

static int threadSort(SortParams *parameters, int count){
    //Performs the threading and makes the function calls 
    // to perform the threaded version of quicksort.

    int errnoTemp, mutRet;

    mutRet = pthread_mutex_init(&countChangeMutex, NULL);

    //Error checks mutex creation.
    if (mutRet != 0){
        errnoTemp = mutRet;
        fprintf(stderr, "ERROR %d: %s\n", errnoTemp, strerror(errnoTemp));
        return errnoTemp;
    }

    //Checks to make sure maximum threads is a valid number.
    if (maximumThreads < 0){
        setSortThreads(0);
    }

    existingThreads = 0;

    threadArr = (pthread_t *)malloc(maximumThreads * sizeof(pthread_t));

    //Error checks malloc for thread array.
    if (threadArr == NULL){
        fprintf(stderr, "%s\n", "Malloc failed, exiting...");
        return 1;
    }

    quickSort(parameters);    

    free(threadArr);

    return 0;

}

void sortThreaded(char** array, unsigned int count) {
    //Uses multithreading to perform a faster quicksort.

    SortParams parameters;
    parameters.array = array; parameters.left = 0; parameters.right = count - 1;
    //threadlessQuicksort(&parameters);
    threadSort(&parameters, count);
}


