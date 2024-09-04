//Jesse A. Jones
//5 Sep, 2022
//CS 360
//Assignment I
//V: 1.11

#include<stdio.h>
#include"hashTable.h"
#include"getWord.h"
#include"wordPairLinkedList.h"
#include"crc64.h"
#include<string.h>
#include<stdlib.h>
//All inclusions necessary to make main work.

int main(int argc, char *argv[]){
	//This function takes in argc and argv 
	// and runs the whole program based on them.

	int numItemsPrint;
	//Tracks number of items to print in the final hash table.

	struct HashTable *mainHash = NULL;
	//Declares hash table to be used throughout the program.

	if (argc == 1){
		fprintf(stderr, "%s\n%s\n", "No arguments given to the program!", 
			"Please follow this format for calling this program:\n./wordpairs <-count> fileNameOne <fileNameTwo> <fileNameThree> ...");
		//Prints error message for no arguments being given.

		exit(1);
		//Exits program with code 1.

	}
	//Checks to see if any arguments were provided.

	for (int i = 1; i < argc; i++){
		if (i == 1){
			if (argv[1][0] == '-'){
				sscanf(argv[1], "%d", &numItemsPrint);
				//Gets count from first argument.

				numItemsPrint = numItemsPrint * -1;
				//The dash character in the count argument is taken as a negative sign 
				// by sscanf and therefore the number must be inverted.

				if (numItemsPrint == 0){
					fprintf(stderr, "%s '%s' %s\n%s\n", 
						"The count argument: ", 
						argv[1], 
						" is invalid.", 
						"Please follow this format for calling this program:\n./wordpairs <-count> fileNameOne <fileNameTwo> <fileNameThree> ...");
					//Prints error message for an incorrect count.

					exit(1);	
				}
				//Accounts for if the count given is invalid.

				if (argc == 2){
					fprintf(stderr, "%s\n%s\n", "Only a count was given when this program was called!", 
						"Please follow this format for calling this program:\n./wordpairs <-count> fileNameOne <fileNameTwo> <fileNameThree> ...");
					//Error is printed if only a count argument is given.

					exit(1);
				}
				//Accounts for if only a count was provided by the user.

			}
			//Handles case if first argument is a user provided count.

			else{
				numItemsPrint = -1;
				//Sets to special case number if count is not given.

				FILE *fp = fopen(argv[1], "r");
				//Opens given file.

				if (fp != NULL){
					mainHash = mainHashFunction(fp, mainHash);
					//Calls function that inserts all word pairs 
					// of the file to the hash table.
		
					fclose(fp);
					//Closes file after all pairs inserted.
				}
				//Accounts for if file is valid.

				else{
					fprintf(stderr, "%s '%s' %s\n%s\n", 
						"The file: ", 
						argv[1], 
						" is invalid.", 
						"Please follow this format for calling this program:\n./wordpairs <-count> fileNameOne <fileNameTwo> <fileNameThree> ...");
					//Prints error message for an incorrect file.

					exit(1);	
				}
				//Handles case of invalid file.

			}
			//Iterates through the given file if first argument isn't a count.

		}
		//If the first argument is a count, 
		// it sets the count number 
		// to the one provided in the argument.

		//If count is not provided, 
		// the print number is set to the special number -1.

		else{
			FILE *fp = fopen(argv[i], "r");
			//Opens given file.

			if (fp != NULL){
				mainHash = mainHashFunction(fp, mainHash);
				//Calls function that inserts all word pairs 
				// of the file to the hash table.
	
				fclose(fp);
				//Closes file after all pairs inserted.
			}
			else{
				fprintf(stderr, "%s '%s' %s\n%s\n", 
					"The file: ", 
					argv[i], 
					" is invalid.", 
					"Please follow this format for calling this program:\n./wordpairs <-count> fileNameOne <fileNameTwo> <fileNameThree> ...");
				//Prints error message for an incorrect file.

				exit(1);
			}
			//Iterates through file and adds word pairs to hashtable 
			// if the file is valid and throws an error if not.

		}
		//This iteration occurs for the remaining arguments.

	}
	//Iterates through arguments and makes the appropriate calls.

	sortAndPrintHash(mainHash, numItemsPrint);
	//Sorts hash table using qsort() and prints all word pairs 
	// in descending order of occurance.

	delTable(mainHash);
	//Deletes hashtable once it has served its purpose.

	exit(0);
	//Exits the program with code 0, indicating a successful run.

}