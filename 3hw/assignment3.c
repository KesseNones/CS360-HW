//Jesse A. Jones
//19 Sep, 2022
//CS 360
//Assignment III
//V: 1.1

#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>
//Includes necessary for this to work.

char *_expandDir(char *path, char *addition){
	//This function adds the given string 
	// to the path passed in, 
	// creating a greater depth file path.

	char *biggerDir = malloc((strlen(path) + 1 + strlen(addition) + 1));
	//Creates space in temp for the present string, 
	// a slash character, the addition string, 
	// and the null terminator.

	biggerDir[0] = '\0';
	//Sets first position to null terminator for strcpy.

	char *slash = "/";
	//Used in concatenation of path names.

	strcpy(biggerDir, path);
	//Copies current directory to temp.

	strcat(biggerDir, slash);
	//Adds first slash to new directory string.

	strcat(biggerDir, addition);
	//Concatenates addition string to current directory. 

	return biggerDir;
}

int isValidRegFile(char *inputPath){
	//This function takes in a path and sees 
	// if it's a valid regular file 
	// that provides read access.

	struct stat statMem, *st = &statMem;
	//Used in determining type of path and if it's accessible for reading.

	int canParse, isRegFile, isReadable;
	//Variables used to hold results of calling functions 
	// and other logic to create more detailed error code returns.	

	canParse = (stat(inputPath, st)) == 0;
	//Checks if the given path is parsable on a basic level of it existing.

	if (!(canParse)){
		return 1;
	}
	//Returns error of 1, indicating path cannot be parsed.

	isRegFile = S_ISREG(st->st_mode);
	//Checks if the path is a regular file.

	if (!isRegFile){
		return 2;
	}
	//Returns error code of 2, indicating the path is not a regular file.

	isReadable = (access(inputPath, R_OK)) == 0;
	//Checks if file has read permissions.

	if (!isReadable){
		return 3;
	}
	//Returns error code of 3 if file cannot be read.

	return 0;
	//Returns 0 for success if the path is a valid file.

}

int isValidDir(char *inputPath){
	//This function checks to see 
	// if the path provided 
	// is a valid readable directory.

	struct stat statMem, *st = &statMem;
	//Used in determining type of path and if it's accessible at all.

	int canParse, isDirectory, isReadable;
	//Variables used to hold results 
	// of function calls in the analysis 
	// of a passed in path.

	canParse = (lstat(inputPath, st)) == 0;
	//Calls stat function to get the stat struct set up for the given path.
	// Checks if something went wrong with this.

	if (!(canParse)){
		return 1;
	}
	//Returns an error code of 1, indicating the path cannot be parsed.

	isDirectory = S_ISDIR(st->st_mode);
	//Checks if the given path is a directory.

	if (!isDirectory){
		return 2;
	}
	//Returns an error code of 2 if the path is not a directory.

	isReadable = (access(inputPath, R_OK)) == 0;
	//Calls the access function to see if the path is readable.

	if (!(isReadable)){
		return 3;
	}
	//Returns error code of 3 if the directory 
	// does not have read permissions. 

	return 0;
	//Zero is returned, indicating the path is parsable, 
	// is a directory, and has read or execute permissions.	

}

int readable(char *inputPath) {
	//This function takes in a directory 
	// and recursively counts all 
	// the valid accessable files within it.

	char dotOne[256] = ".";
	char dotTwo[256] = "..";
	//Used in checking for a certain directory file name.

	char *enlargedPath = NULL;
	//Used to hold the expanded path.

	int validFileCount = 0;
	//Used to count the number of valid regular files.

	int errorCode = 0;
	//Used to store errno for returns.

	int callResult;
	//Used to track recursive function returns.

	DIR *currentDir;
	//Used in interacting with the current directory file.

	if (inputPath == NULL){
		inputPath = getcwd(inputPath, PATH_MAX);
		//Gets the current working directory path.

		char temp[PATH_MAX];
		//Creates space for the CWD to be copied over.

		strcpy(temp, inputPath);
		//Copies current directory to temp.

		free(inputPath);
		//Frees malloc made by getcwd.

		inputPath = temp;
		//Sets inputPath to CWD.

	}
	//Accounts for case if there is no input path passed in (NULL).

	struct dirent *iNode;
	//Creates pointer to inode given from readdir().

	int fileCheck = isValidRegFile(inputPath);
	//Checks to see if the path is a valid regular file.
	// errno also updates based on what went wrong. 

	if (fileCheck == 0){
		return 1;
	}
	//Handles case of if path ends with a valid regular file.

	else if(fileCheck == 3){
		return 0;
	}
	//Handles case of a regular file being valid but not readable.

	else{
		errorCode = errno;
	}
	//Sets the error code variable to errno since the file is not valid.

	int dirCheck = isValidDir(inputPath);
	//Calls function to see if the given path is a valid directory.

	if (dirCheck == 0){
		currentDir = opendir(inputPath);
		//Creates directory stream.

		if (currentDir == NULL){
			errorCode = errno;
			return -errorCode;
		}
		//Handles case of opendir failure.

		iNode = readdir(currentDir);
		//Reads an iNode from directory stream 
		// or NULL if it's an empty/invalid directory.

		while (iNode != NULL){
			if (((iNode->d_type) == DT_DIR) || ((iNode->d_type) == DT_REG)){
				if (((strcmp(dotOne, iNode->d_name)) != 0) && ((strcmp(dotTwo, iNode->d_name)) != 0)){
					enlargedPath = _expandDir(inputPath, iNode->d_name);
					//Expands directory to include new directory chunk.

					char temp[strlen(enlargedPath) + 1];
					//Used to move bigger path from heap to stack.

					strcpy(temp, enlargedPath);
					free(enlargedPath);
					enlargedPath = temp;
					//Moves bigger path from the heap to the stack.

					callResult = readable(enlargedPath);
					//Saves call result of recursive call.

					if (callResult >= 0){
						validFileCount += callResult;
					}
					//Increments validFileCount by the number 
					// of valid files in the directory or file itself,
					// if the resulting call is not less than 0,
					// since that would be an error.

				}
				//Makes recursive call if name is not "." or "..".

			}
			//Makes a recursive call if a directory or file 
			// that isn't named "." or ".." is found.

			iNode = readdir(currentDir);
			//Moves on to next element in directory, 
			// or NULL if there are no others.

		}
		//Iterates through a directory, counting valid regular files 
		// and making recursive calls when sub-directories are found.

		closedir(currentDir);
		//Closes directory stream after iteration completes.

		return validFileCount;
		//Returns up stack with the count found or a negative error code.

	}
	//Handles case of a valid directory being passed in.

	if (dirCheck > 0){
		errorCode = errno;
	}
	//If the directory is not valid, 
	// the error code variable is set 
	// to the current value of errno.

    return -errorCode;
    //Returns a negative error code if the item being looked 
    // at is not a valid directory or regular file.
}
