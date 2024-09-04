//Jesse A. Jones
//2 Nov, 2022
//CS 360
//Assignment VII
//V: 1.05

#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include"assignment7.h"

#define MAX_THREADS 4

typedef struct _sortParams {
    char** array;
    int left;
    int right;
} SortParams;

char **createStringArr(int size){
	//Creates string array of size size.
	char **strArr = (char **)calloc(size, sizeof(char *));
	return strArr;
}

void deleteStringArr(char **delArr, int size){
	//Deletes string array passed in.

	for (int i = 0; i < size; i++){
		free(delArr[i]);
	}
	free(delArr);
}

void fileToStringArray(char **strArr, int size, int fd){
	//Transfers words from a file to the string array of choice. 

	char buf[1024] = {'\0'};
	char miniBuf[2] = {'\0'};
	int bufIndex = 0;
	int wordsRead = 0;
	char *insertString;

	while (wordsRead < size){
		while(miniBuf[0] != '\n' && miniBuf[0] != ' '){
			if(read(fd, miniBuf, 1) == 0){
				break;
			}
			//printf("{{{{%s\n", miniBuf); //TEST

			//if (miniBuf[0] == '\n') printf("{{{{%s\n", "/n"); //TEST
			//if (miniBuf[0] == ' ') printf("{{{{%s\n", "[SPACE]"); //TEST

			if (miniBuf[0] != '\n' && miniBuf[0] != ' '){
				buf[bufIndex] = miniBuf[0];
				bufIndex++;
			}
		}
		buf[bufIndex] = '\0';
		miniBuf[0] = '\0';

		insertString = strdup(buf);
		strArr[wordsRead] = insertString;
		bufIndex = 0;
		wordsRead++;
	}



}

double getTime() {
	//Stolen from code challenge, not mine! 
	// Used to time how long functions run.
    struct timespec theTime;
    if (clock_gettime(CLOCK_MONOTONIC, &theTime) < 0) {
        fprintf(stderr,"Failed to obtain time: %s\n",strerror(errno));
        exit(1);
    }
    double result = theTime.tv_sec;
    result += theTime.tv_nsec * 1.0E-9;
    return result;
}

int compareSorts(char **strArr1, char **strArr2, int size){
	for (int i = 0; i < size; i++){
		if (strcmp(strArr1[i], strArr2[i]) != 0){
			return 1;
		}
	}
	return 0;
}

int determineIfSorted(char **strArr, int size){
	for (int i = 0; i < size - 1; ++i){
		if (strcmp(strArr[i], strArr[i + 1]) > 0){
			return 0;
		}
	}
	return 1;
}

int qCompare(const void *string1, const void *string2){
	return (strcmp((char *)string2, (char *)string1));
}

int main(int argc, char const *argv[]){
	double start, end;

	int strArrSize = 100000;

	//int strArrSize2 = 100;

	//int arrCompResult;

	int isSorted;

	int fd = open("lorem", O_RDONLY);

	//int fd2 = open("lorem", O_RDONLY);

	char **stringArr = createStringArr(strArrSize);
	if (stringArr == NULL){
		fprintf(stderr, "%s\n", "Well shit");
		exit(1);
	}

	// char **stringArr2 = createStringArr(strArrSize2);
	// if (stringArr2 == NULL){
	// 	fprintf(stderr, "%s\n", "Well shit");
	// 	exit(1);
	// }

	fileToStringArray(stringArr, strArrSize, fd);

	//fileToStringArray(stringArr2, strArrSize2, fd2);

	close(fd);
	//close(fd2);

	isSorted = determineIfSorted(stringArr, strArrSize);
	printf("%d\n", isSorted);

	//arrCompResult = compareSorts(stringArr, stringArr2, strArrSize);

	//printf("%d\n", arrCompResult);

	// for (int i = 0; i < strArrSize2; i++){ //TEST
	// 	printf("STRING: %s SIZE: %ld\n", stringArr2[i], strlen(stringArr2[i]));

	// 	// for (int j = 0; j < strlen(stringArr[i]); j++){
	// 	// 	printf("%c\n", stringArr[i][j]);
	// 	// 	if (stringArr[i][j] == ' ') printf("SPACE\n");
	// 	// }
	// }

    //setSortThreads(1000);

	// for (int i = 0; i < strArrSize; i++){ //TEST
	// 	printf("STRING: %s SIZE: %ld\n", stringArr[i], strlen(stringArr[i]));
	// }

	// printf("----------------------\n");

	// for (int i = 0; i < strArrSize2; i++){ //TEST
	// 	printf("STRING: %s SIZE: %ld\n", stringArr2[i], strlen(stringArr2[i]));
	// }

	setSortThreads(MAX_THREADS);

	start = getTime();

	sortThreaded(stringArr, strArrSize);

	end = getTime();

	printf("MY TIME: %f\n", end - start);

	isSorted = determineIfSorted(stringArr, strArrSize);
	printf("%d\n", isSorted);

	//arrCompResult = compareSorts(stringArr, stringArr2, strArrSize);

	//printf("%d\n", arrCompResult);

	// for (int i = 0; i < strArrSize; i++){ //TEST
	// 	printf("STRING: %s SIZE: %ld\n", stringArr[i], strlen(stringArr[i]));
	// }

	// printf("----------------------\n");

	// for (int i = 0; i < strArrSize2; i++){ //TEST
	// 	printf("STRING: %s SIZE: %ld\n", stringArr2[i], strlen(stringArr2[i]));
	// }	

	deleteStringArr(stringArr, strArrSize);

	//deleteStringArr(stringArr2, strArrSize2);

	

	return 0;
}
