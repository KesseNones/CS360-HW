//Jesse Jones
//13 Sep, 2022
//V: 1.12

#include "lineNum.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main(){
	int poop;
	char *query = "queen";
	poop = lineNum("webster_16", query, 16);
	if (poop < 0){
		//fprintf(stderr, "%s\n", strerror(errno));
		printf("FAILED; Last Searched: %d\n", poop);
	}
	else{
		printf("%s exists on line: %d\n", query, poop);
	}


	return 0;
}