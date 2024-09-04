//Jesse A. Jones
//13 Sep, 2022
//CS 360
//Assignment II
//V: 1.12

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int _stringLength(char *str){
	//A helper function that finds the length of a string passed in.
	// This is mostly used in the writing of errors to stderr.

	int length = 0;
	//Used in finding the length of the string.

	for (int i = 0; str[i] != '\0'; i++){
		length++;
	}
	//Increases length for each non 
	// null terminator character in string.

	return length;
}

int _wordFind(int fd, char *word, int dictWidth){
	//Uses binary search on a given dictionary file 
	// to find a line number the input word exists on. 
	// Returns a negative line number if no word is found 
	// or an error code if there was a failure to read a line.

	int readBytes;
	//Tracks the number of bytes read. Useful for read error handling.

	int errnoTemp;
	//Used to save the current value 
	// of errno to use in fprintf 
	// if an error occurs.

	char errnoString[(sizeof(int) + 1) * 8];
	//Used in the conversion of errno from an int to a string.

	char buffer[dictWidth];
	//Creates a character buffer the length of the dictionary width specified.

	char *errBuff;
	//Used in writing to stderr.

	char *newLine = "\n";
	//Used in writing to stderr.

	int len;
	//Used to find the length of the error string.

	long dictIndex, maxIndex, minIndex;
	//Used in lseek operations to track 
	// the offset in fd and to perform binary search.

	int comp;
	//Used in string comparisons.

	maxIndex = lseek(fd, 0, SEEK_END);
	//Sets maxIndex to the last byte of the file.

	if (maxIndex == 0){
		return -1;
	}
	//Accounts for if file is empty.

	maxIndex /= dictWidth;
	//Scales maxIndex to a word postion from number of bytes.
	// This yields an index that is one greater than the final index.

	minIndex = -1;
	//Sets the minimum word index to -1, 
	// before the first word of the file.

	while (1){
		if ((maxIndex - minIndex) == 1){
			break;
		}
		//Loops ends if there are no words 
		// between the minimum and maximum indicies left, 
		// indicating a failure to find the word requested.

		dictIndex = (((maxIndex - minIndex) / 2) + minIndex);
		//Sets the index in the file to be halfway 
		// between the maximum and minimum file indicies.

		lseek(fd, (dictIndex * dictWidth), SEEK_SET);
		//Sets offset to byte position at the start 
		// of the word at the given index.

		readBytes = read(fd, buffer, dictWidth);
		//Reads the word at the specified position.

		if ((readBytes != 0) && (readBytes != dictWidth)){
			errnoTemp = errno;

			sprintf(errnoString, "%d ", errnoTemp);
			//Turns errno to a string for writing to stderr.

			len = _stringLength(errnoString);
			//Gets length of errno string using homemade helper function.

			write(2, errnoString, len);
			//Writes errno to stderr.

			errBuff = strerror(errnoTemp);
			//Sets pointer to appropriate error string.

			len = _stringLength(errBuff);
			//Gets length of error string using 
			// home made string length function.

			write(2, errBuff, len);
			//Writes error buffer to stderr.

			lseek(2, 0, SEEK_END);
			//Goes to the last byte of stderr.

			write(2, newLine, 2);
			//Writes newline and null terminator to stderr, 
			// completing the error message. 

			return errno;
		}
		//Accounts for failure of read() to read every byte of a line.

		buffer[dictWidth - 1] = '\0';
		//Adds null terminator to the string that was read.

		comp = strcmp(word, buffer);
		//Compares word to be found with the current word.

		if (comp == 0){
			return (dictIndex + 1);
		}
		//If the searched word is found, 
		// return the line number of the word.

		else if (comp > 0){
			minIndex = dictIndex;
		}
		//Sets minimum index to current index, 
		// since the word being searched 
		// is larger than the current one.

		else{
			maxIndex = dictIndex;
		}
		//Sets maximum index to current index 
		// since the word being searched 
		// is smaller than the current one.

	}
	//Iterates through dictionary to find the input word string.
	// Returns line number of successfull, breaks if not.

	return -(dictIndex + 1);
	//Returns the negative of the last line searched, 
	// indicating a failure to find the requested word.

}

int lineNum(char *dictionaryName, char *word, int dictWidth) {
	//This function takes in a dictionary file, 
	// a word to find, and dictionary width to work with.
	// This function then opens the file and uses a helper 
	// function to search for the word.
	//Returns line number of found word, 
	// the negative of the line last searched if no word could be found, 
	// or returns an error code if the file could not be opened 
	// or the word of a given line could not be read. 

	int fd;
	//Integer used for file descriptor.

	int wordLine;
	//Used to receive the result of the helper function that seeks 
	// a word line number or a negative number/error code if there are issues.

	char formatWord[dictWidth];
	//Used to copy over the passed in string 
	// to the appropriate dictionary format.

	int len;
	//Used to receive the result of calling the _stringLength function.

	char errnoString[(sizeof(int) + 1) * 8];
	//Used in the conversion of errno from an int to a string.

	char *errBuff;
	//Used in the writing error message to stderr.

	char *newLine = "\n";
	//This string is written to the end of an error message in stderr.

	int errnoTemp;
	//Temporary place to save errno value for fprintf.

	for (int i = 0; i < dictWidth; i++){
		formatWord[i] = ' ';
	}
	//Sets every character in formatted string to space character.

	for (int j = 0; ((word[j] != '\0') && (j < dictWidth)); j++){
		formatWord[j] = word[j];
	}
	//Copies all characters up to the dictionary 
	// width from the input word to the formatted word char array.

	formatWord[dictWidth - 1] = '\0';
	//Sets last character of the formatted version
	// of word to the newline character.

	fd = open(dictionaryName, O_RDONLY);
	//Opens given file for reading only.

	if (fd < 0){
		errnoTemp = errno;

		sprintf(errnoString, "%d ", errnoTemp);
		//Turns errno to a string for writing to stderr.

		len = _stringLength(errnoString);
		//Gets length of errno string using homemade helper function.

		write(2, errnoString, len);
		//Writes errno to stderr.

		errBuff = strerror(errnoTemp);
		//Sets pointer to appropriate error string.

		len = _stringLength(errBuff);
		//Gets length of error string using 
		// home made string length function.

		write(2, errBuff, len);
		//Writes error buffer to stderr.

		lseek(2, 0, SEEK_END);
		//Goes to the last byte of stderr.

		write(2, newLine, 2);
		//Writes newline and null terminator to stderr, 
		// completing the error message. 

		return errno;
	}
	//Accounts for if the opening of the file failed.

	wordLine = _wordFind(fd, formatWord, dictWidth);
	//Calls function that employs binary search 
	// to find the line number the formatWord exists in.

	close(fd);
	//Closes file once _wordFind() has either found a word or not.

	return wordLine;
}
