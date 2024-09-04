#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
//Inclusions necessary to make code work.

//Jesse A. Jones
//26 Sep, 2022
//CS 360
//Assignment IV
//V: 1.1

int colonLocator(int argc, char *argv[]){
	//This function locates the colon character in argv.
	// Returns index of colon on success and 0 on failure.

	for (int i = 1; i < argc; i++){
		if (argv[i][0] == ':'){
			return i;
		}
	}
	//Iterates through argv to locate colon character.

	return 0;
}

void childFunc(int argc, char *argv[], int colonLoc, int *fd){
	//This function performs the actions of the child end of the pipe 
	// and executes the program given to it if it's valid.

	int isWeird = 0;
	//Tracks if colon is in position 1, indicating no first program provided.

	int isSingleCommand = 0;
	//Used for edge case of a single command 
	// being passed into the first argument 
	// with no second argument present.

	int errnoTemp, result;
	//Used in saving errno and error checking respectively.

	if (colonLoc == 0){
		colonLoc = argc;
		isSingleCommand = 1;
	}
	//Sets colon location to end 
	// of the argument string 
	// since no colon exists.

	if(colonLoc == 1){
		isWeird = 1;
		colonLoc = 2;
	} 
	//Indicates edge case is occuring where no first argument 
	// is given but there is still a colon before arg 2.
	//Sets colonloc to valid position.

	if (colonLoc == 2){
		isSingleCommand = 1;
		colonLoc = 2;
	}
	//Checks for if input has a single command 
	// in the argument followed by the colon character.

	char *firstArgStringArr[colonLoc];
	//Creates an array that contains the argument array 
	// of the function to be executed by the child.

	int reader = fd[0];
	//Sets input section of pipe to a more readable variable.

	int writer = fd[1];
	//Sets output section of pipe to a more readable variable.

	if (!isSingleCommand){
		result = close(reader);
		//Closes input of pipe since it is not needed.

		if (result != 0){
			errnoTemp = errno;
			fprintf(stdout, "%s\n", strerror(errnoTemp));
			fflush(stdout);
			exit(errnoTemp);
		}
		//Handles case of failure of closing input of pipe.

		result = close(1);
		//Closes stdout.

		if (result != 0){
			errnoTemp = errno;
			fprintf(stdout, "%s\n", strerror(errnoTemp));
			fflush(stdout);
			exit(errnoTemp);
		}
		//Handles case of failure of closing stdout.

		result = dup(writer);
		//Duplicates writing end of pipe to stdout.

		if (result == -1){
			errnoTemp = errno;
			fprintf(stdout, "%s\n", strerror(errnoTemp));
			fflush(stdout);
			exit(errnoTemp);
		}
		//Error checks dup.

		result = close(writer);
		//Closes origional writing pipe section.

		if (result != 0){
			errnoTemp = errno;
			fprintf(stdout, "%s\n", strerror(errnoTemp));
			fflush(stdout);
			exit(errnoTemp);
		}
		//Error checks close.

	}
	//Piping occurs if there is a second argument 
	// that may need the data from the first argument.

	for (int i = 1; i < colonLoc; i++){
		if (!isWeird){
			firstArgStringArr[i - 1] = argv[i];
		}
		//Transfers string from argv to the arg array if things are normal.

		else{
			firstArgStringArr[i - 1] = "";
		}
		//Handles case of the colon being in a weird spot.

	}
	//Transfers strings from argv if things are normal.

	firstArgStringArr[colonLoc - 1] = NULL;
	//NULL end added for execvp call.

	if (!isWeird){
		result = execvp(firstArgStringArr[0], firstArgStringArr);
		//Given program executed.

		if (result == -1){
			errnoTemp = errno;
			fprintf(stdout, "%s\n", strerror(errnoTemp));
			fflush(stdout);
		}
		//Prints error if something went wrong with execution.
	}
	//Executes if there is something to execute.

}

void parentFunc(int argc, char *argv[], int colonLoc, int *fd){
	//This function performs the actions of the parent end of the pipe.

	if (colonLoc == 0){
		wait(NULL);
		exit(0);
	}
	//Waits for child process to finish before exiting 
	// in the case of there being only one program to execute.

	int secondArgArrSize = argc - colonLoc;
	//Finds size of the second argument string. 

	int isWeird = 0;
	//Used to track if an edge case is happening.

	if (secondArgArrSize == 1){
		isWeird = 1;
		secondArgArrSize = 2;
	}
	//Sets arg size of the second argument 
	// to something reasonable while indicating 
	// that an edge case involving there being no command 
	// to execute in the second argument is happening.

	char *secondArgStringArr[secondArgArrSize];
	//Creates an array that contains the argument array 
	// of the function to be executed by the parent.

	int result;
	//Used in function call results.

	int errnoTemp;
	//Used to store errno value.

	int reader = fd[0];
	//Sets input section of pipe to a more readable variable.

	int writer = fd[1];
	//Sets output section of pipe to a more readable variable.

	result = close(writer);
	//Closes writing portion of pipe.

	if (result != 0){
		errnoTemp = errno;
		fprintf(stdout, "%s\n", strerror(errnoTemp));
		fflush(stdout);
		exit(errnoTemp);
	}
	//Handles the failure case involving closing the output of the pipe.

	result = close(0);
	//Closes stdin.

	if (result != 0){
		errnoTemp = errno;
		fprintf(stdout, "%s\n", strerror(errnoTemp));
		fflush(stdout);
		exit(errnoTemp);
	}
	//Handles case involving a failure to close stdin.

	result = dup(reader);
	//Duplicates reading portion of pipe to stdin.

	if (result == -1){
		errnoTemp = errno;
		fprintf(stdout, "%s\n", strerror(errnoTemp));
		fflush(stdout);
		exit(errnoTemp);
	}
	//Handles failure case of dup.

	result = close(reader);
	//Closes original input pipe section.

	if (result != 0){
		errnoTemp = errno;
		fprintf(stdout, "%s\n", strerror(errnoTemp));
		fflush(stdout);
		exit(errnoTemp);
	}
	//Handles failure case of closing the input end of the pipe.

	for (int i = colonLoc + 1; i < argc; i++){
		secondArgStringArr[i - (colonLoc + 1)] = argv[i];
	}
	//Transfers strings from argv to arg array 
	// to be potentially used in execvp.

	if (isWeird){
		secondArgStringArr[0] = "";
	}
	//Sets first argument to an empty string if an edge case 
	// is happening where there is nothing 
	// past the colon in the provided argument.

	secondArgStringArr[(secondArgArrSize) - 1] = NULL;
	//NULL end added for execvp call.

	wait(NULL);
	//Waits for child to finish running or be terminated by something else.

	if (!isWeird){
		result = execvp(secondArgStringArr[0], secondArgStringArr);
		//Executes given program.

		if (result == -1){
			errnoTemp = errno;
			fprintf(stdout, "%s\n", strerror(errnoTemp));
			fflush(stdout);
		}
		//Prints error if something went wrong with execution.
	}
	//If there exists something to execute, execute it.

}

int main(int argc, char *argv[]){
	//This is where the piping and forking happens in the program, 
	// as well as the necessary function calls.

	if (argc <= 2){
		if (argc == 1){
			exit(0);
		}
		//Exit if no arguments are given.

		if ((argc == 2) && argv[1][0] == ':'){
			exit(0);
		}
		//Exit if only a colon is given as an argument.

	} 
	//Accounts for if no arguments were given.

	int colonLoc = colonLocator(argc, argv);
	//Uses function to find position of colon in argv.

	int fd[2];
	//Used to track the reading and writing file descriptors.

	int errnoTemp;
	//Used to save errno.

	if (pipe(fd) < 0){
		errnoTemp = errno;
		fprintf(stdout, "%s\n", strerror(errnoTemp));
		fflush(stdout);
		exit(errnoTemp);
	}
	//Handles case of pipe failing to form.

	if (fork()){
		parentFunc(argc, argv, colonLoc, fd);
	}
	//Handles case involving parent end of pipe.

	else{
		childFunc(argc, argv, colonLoc, fd);
	}
	//Handles case involving child end of pipe.

	return 0;
}