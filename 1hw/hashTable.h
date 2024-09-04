//This is part of the word pair program written by Jesse Jones.

#ifndef HASHTBL_H
#define HASHTBL_H
//Accounts for double inclusion.

#include <stdio.h>

struct HashTable{
	//This struct contains the hashtable data structure 
	// and its relevant information.
	
	struct LinkedList **hash;
	//This double pointer stores an array of pointers 
	// to the linked list struct, 
	// effectively acting as an array of linked lists.

	int hashSize;
	//Tracks how big the hash table is.

	int valCount;
	//Tracks how many nodes are in the hash table.

	//Both hashSize and valCount are used in calculating a load factor.

};

struct HashTable *createHashTable(int hashSize);
//This function creates an empty hash table of a specified size.

//Returns NULL if hash table creation fails.

int insertPairToHash(struct HashTable *hashTbl, char *insPair, int freqCount);
//Uses several provided and made functions 
// to insert a word pair into the passed in hash table 
// or update the occurance count of a word pair already 
// in the hash table if a match is found.

//Returns 1 if insertion failed.

struct HashTable *resizeTable(struct HashTable *hashTbl);
//Resizes the input hashtable by a factor of three to reduce collisions.

//Returns NULL if resize failure occurs.

int delTable(struct HashTable *hashTbl);
//Deletes the passed in hash table.

//Returns 1 if deletion fails.

struct HashTable *mainHashFunction(FILE *fp, struct HashTable *hashTbl);
//Inserts contents of a file into a hashtable if one already exists 
// and creates one if one does not exist.

//Throws errors if one of its called functions fails and exits the program.

void sortAndPrintHash(struct HashTable *hash, int printNumber);
//A function that takes in a hashtable and
// applies the qsort() function to an array 
// of nodes containing word pairs from the hash table. 
// This sorted array is then printed the number 
// of times specified by printNumber 
// or all of it if printNumber is -1. 
// The printed result is every word pair from the hash table 
// and the number of times each one has occured in descending order.

//Throws an error if something goes wrong and exits the program. 

int _freqCompar(const void *node1, const void *node2);
//A helper function that is used in the comparison 
// of frequency between one node and the other in the qsort() function.

#endif