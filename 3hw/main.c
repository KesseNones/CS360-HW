//Jesse Jones
//19 Sep, 2022
//V: 1.1

#include<stdio.h>
#include"assignment3.h"
#include<errno.h>

#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>

#include<stdio.h> //DESTROY LATER?
#include<assert.h>


int main(){
	int fileCount;

	fileCount = readable(NULL);
	printf("%d\n", fileCount);
	fprintf(stderr, "%d: %s\n", errno, strerror(errno));

	fprintf(stderr, "|||%s\n", strerror(11));

	return 0;
}