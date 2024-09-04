#ifndef  ASS_III_H
#define ASS_III_H

int readable(char *inputPath);
//Counts readable files.

int isValidRegFile(char *inputPath);
//Sees if the path provided is a valid and readable regular file.

int isValidDir(char *inputPath);
//Checks to see if the path provided is a valid directory that can be read.

char *_expandDir(char *path, char *addition);
//Adds additional directory to current path.

#endif