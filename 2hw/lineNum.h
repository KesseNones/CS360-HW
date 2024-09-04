#ifndef ASS_II_H
#define ASS_II_H

int lineNum(char *dictionaryName, char *word, int dictWidth);
//Gets the line number of a requested dictionary word 
// or a negative if the word does not exist.

int _wordFind(int fd, char *word, int dictWidth);
//A helper function that employs binary 
// search through a dictionary to find a word.

int _stringLength(char *str);
//Homemade helper function that finds length of string.

#endif