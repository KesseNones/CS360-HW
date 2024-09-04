//This is part of the word pair program written by Jesse Jones.

#include "hashTable.h"
#include "wordPairLinkedList.h"
#include "crc64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getWord.h"
//All inclusions necessary for hashTable.c

#define HASH_START_SIZE 243
//Picked 243 because it is a power of three, making the scaling more efficient 
// since the entire hash table is scaled by a factor of three during resizing.

struct HashTable *createHashTable(int hashSize){
	if (hashSize <= 0){
		return NULL;
	}
	//Null is returned if an invalid hashtable size is given.

	struct HashTable *hashTbl = (struct HashTable *)malloc(
		sizeof(struct HashTable) * hashSize);
	//Creates a hash table with a size of the size inputted.

	if (hashTbl == NULL){
		return NULL;
	}
	//Returns null if malloc for hash table struct did not work.

	hashTbl->hashSize = hashSize;
	hashTbl->valCount = 0;
	//Establishes basic fields of hash table.

	struct LinkedList **hashDataStruct = (struct LinkedList **)calloc(
		hashSize, sizeof(struct LinkedList *));
	//Creates a new empty hash table in the form 
	// of an array of pointers to linked lists 
	// that are set to null by calloc.

	if (hashDataStruct == NULL){
		return NULL;
	}
	//Returns null if malloc for the hash table data structure failed.

	hashTbl->hash = hashDataStruct;
	//Sets the hash field to the new hash table.

	return hashTbl;
}

int insertPairToHash(struct HashTable *hashTbl, char *insPair, int freqCount){
	if (hashTbl == NULL || insPair == NULL || freqCount <= 0){
		return 1;
	}
	//Returns 1 if any of the input is invalid.

	unsigned long long hashCode = crc64(insPair);
	//crc64 is used to generate the initial hash code from the word pair.

	int hashCodeIndex = (hashCode % hashTbl->hashSize);
	//Creates an index for the word pair to be inserted at 
	// by modding the hash code by the length 
	// of the hash table and casting it as an int implicitly.	

	struct LinkedList *insLL = hashTbl->hash[hashCodeIndex];
	//Makes a linked list pointer that points to the index of the hash table.

	struct Node *newNode;
	//Used in the creation of a new node.

	int callResult;
	//Tracks results of function calls.

	if (insLL == NULL){
		newNode = createNode(insPair, freqCount, NULL);
		//Creates a node with the word pair, an occurance count of 1, and no next node.

		if (newNode == NULL){
			return 1;
		}
		//Factors if creation of the node failed.

		struct LinkedList *newLL = createLinkedList(NULL);
		//Creates a new empty linked list struct with no head node.

		if (newLL == NULL){
			return 1;
		}
		//Factors in if creation of new linked list failed.

		callResult = insertNode(newLL, newNode);
		//Inserts the new node into the linked list as the head node.

		if (callResult == 1){
			deleteNode(newNode);
			deleteLinkedList(newLL);
			return 1;
		}
		//Factors if inserting a node to the new linked list failed.

		hashTbl->hash[hashCodeIndex] = newLL;
		//Sets bucket in hash table as new linked list.

		hashTbl->valCount++;
		//If insertion of word pair was successfull, 
		// the number of word pairs in the hash table increases.

	}else{
		hashTbl->valCount++;
		//If insertion of word pair was successful, 
		// the number of word pairs in the hash table increases.

		newNode = createNode(insPair, freqCount, NULL);
		//Creates new node to be inserted.

		if (newNode == NULL){
			return 1;
		}
		//Returns 1 if node failed to be created.

		callResult = insertNode(insLL, newNode);
		//Inserts node into linked list assuming it is a unique node.

		if (callResult == 2){
			deleteNode(newNode);
			hashTbl->valCount--;
		}
		//Decrements valCount, cancelling out the increment 
		// since no new word pair was inserted into the table.

		if(callResult == 1){
			deleteNode(newNode);
			return 1;
		}
		//Returns 1 if insertion failed, and deletes the created node.

	}

	return 0;

}

int delTable(struct HashTable *hashTbl){
	for (int i = 0; i < (hashTbl->hashSize); i++){
		if (hashTbl->hash[i] != NULL){
			deleteLinkedList(hashTbl->hash[i]);
			//Empties the given bucket.
		}
	}
	//Iterates through the hash table and empties occupied buckets.

	free(hashTbl->hash);
	//Deletes hash table data structure.

	free(hashTbl);
	//Deletes hash table struct.

	return 0;
}

struct HashTable *resizeTable(struct HashTable *hashTbl){
	if (hashTbl == NULL){
		return NULL;
	}
	//Accounts for if there is no hash table to resize.

	struct HashTable *biggerHash = createHashTable((hashTbl->hashSize * 3));
	//Creates a new hash table scaled up by a factor of three.

	struct Node *cursor;
	//Used in linked list iteration.

	for (int i = 0; i < hashTbl->hashSize; i++){
		if (hashTbl->hash[i] != NULL){
			cursor = (hashTbl->hash[i]->head);
			//Sets cursor to head of linked list.

			while (cursor != NULL){
				insertPairToHash(biggerHash, 
					cursor->value, 
					cursor->occuranceCount);
				//Inserts node information into bigger hash.

				cursor = cursor->next;
				//Iterates to next node in linked list.

			}
			//Iterates through linked list at a given bucket and inserts 
			// the node information into the new bigger hash table.
			
		}
	}
	//Nested loop iterates through the old hash table 
	// and copies all the elements over to the new bigger hash table.

	delTable(hashTbl);
	//Deletes the old table.

	return biggerHash;
}

struct HashTable *mainHashFunction(FILE *fp, struct HashTable *hashTbl){
	char *pairString;
	//Used to capture output of calling the word pair generation function.

	int result;
	//Tracks function call results.

	double loadFactor;
	//Used in calculating the hashtable load factor.

	struct HashTable *hashTableStrct;
	//Declares hashtable pointer that will be used in this function.

	if (hashTbl == NULL){
		hashTableStrct = createHashTable(HASH_START_SIZE);
		//Creates a hashtable with an initial size of the default initial size.
	}else{
		hashTableStrct = hashTbl;
		//Sets the hashtable pointer to the hash table pointer passed in.
	}
	//Accounts for cases if the passed in hashtable being null or not.

	if (hashTableStrct == NULL){
		fprintf(stderr, "%s\n", "Failed to allocate memory for hashtable. Exiting...");
		//Prints error message for failure of hash table creation.

		exit(1);
	}
	//Accounts for failure to create new hash table.

	char *currentWord;
	//Current word pointer declared for creation of word pairs.

	char *previousWord = getNextWord(fp);
	//Fetches a word from the file to be contained in the previous word.

	while (1){
		currentWord = getNextWord(fp);
		//Fetches current word within the file.

		pairString = generateWordPair(previousWord, currentWord);
		//Gets word pair from file pointer.

		if (pairString == NULL){
			free(previousWord);
			free(pairString);
			break;
		}
		//Stops loop if end of file reached.

		free(previousWord);
		previousWord = currentWord;
		//Shifts one word forward in file for the next pair to be made.

		result = insertPairToHash(hashTableStrct, pairString, 1);
		//Inserts a given word pair into the hash table 
		// with an initial frequency count of 1 
		// since it is a potentially new pair to be inserted.

		free(pairString);
		//Frees the string pair after the information it contains 
		// was inserted into the hash table as a new node.

		if (result == 1){
			fprintf(stderr, "%s\n", "Failed to insert word pair to hashTable. Exiting...");
			//Prints error message for failure of hashtable insertion.

			exit(1);
		}
		//Accounts for if the insertion to the hash table failed.

		loadFactor = ((double)(hashTableStrct->valCount)/
			(double)(hashTableStrct->hashSize));
		//Calculates the load factor on the hash table.

		if (loadFactor > 0.70){
			hashTableStrct = resizeTable(hashTableStrct);
			//Resizes hash table by factor of three 
			// if load factor goes beyond 0.7.

			if (hashTableStrct == NULL){
				fprintf(stderr, "%s\n", "Failed to resize hashtable correctly. Exiting...");
				//Prints error message for failure of hashtable resizing.

				exit(1);
			}
			//Accounts for if resize fails.
		}
		//If the load factor becomes greater than 0.7, 
		// the table resizes by a factor of three.
	}

	return hashTableStrct;
}

void sortAndPrintHash(struct HashTable *hash, int printNumber){
	int noPrintLimit = 0;
	//A quasi boolean that is used for 
	// the special case of no count being provided.

	struct Node *pairArray = (struct Node *)malloc(
		sizeof(struct Node) * hash->valCount);
	//Creates space for an array of nodes that is the length 
	// of the number of word pairs in the hash table.

	if (pairArray == NULL){
		fprintf(stderr, "%s\n", "Failed to allocate space for the array to be printed. Exiting...");
		//Prints allocation error.

		exit(1);
	}
	//Throws error and exits program if malloc failed.

	int pairArrIndex = -1;
	//Used in iteration through word pair node array.

	struct Node *cursor;
	//Used in linked list iteration.

	for (int i = 0; i < hash->hashSize; i++){
		if (hash->hash[i] != NULL){
			cursor = hash->hash[i]->head;
			//Puts cursor at head of linked list in the given bucket.

			while (cursor != NULL){
				pairArrIndex++;
				//Moves to next position in the pairArray.

				pairArray[pairArrIndex].occuranceCount = cursor->occuranceCount;
				//Copies occurance count of a given word pair node 
				// in the bucket to the array to be sorted.
				
				pairArray[pairArrIndex].next = NULL;
				//Next is set to null for every node in the array 
				// because it is a pointer that is not needed.

				pairArray[pairArrIndex].value = strdup(cursor->value);
				//Duplicates the word pair into the node 
				// in the array from the node in the hashtable.

				cursor = cursor->next;
			}
			//Iterates through linked list of given bucket 
			// and inserts into array to be qsorted.
		}
	}
	//This nested loop iterates through the hash table and the pairArray 
	// to transfer the information of each hashtable node 
	// to the array that will be sorted.
	
	qsort(pairArray, 
		hash->valCount, 
		sizeof(struct Node), 
		_freqCompar);
	//Sorts pair array in descending order 
	// of occurance of each word pair.

	if (printNumber == -1){
		noPrintLimit = 1;
	}
	//Setting noPrintLimit to 1 makes it 
	// so the printing is not prematurely cut off.
	//This is for if there is no count provided by the user in main.

	for (int i = 0; i < hash->valCount; i++){
		if (printNumber > 0 || noPrintLimit){
			printf("%10d %s\n", 
				pairArray[i].occuranceCount, 
				(char *)pairArray[i].value);
			//Prints word pair and number of times it has occured.
			
		}
		//Prints if there are still allowed prints left 
		// or if there is no print limit.

		free(pairArray[i].value);
		//Frees word pair pointer of given node.

		if (!noPrintLimit){
			printNumber--;	
		}
		//Decrements the number of prints remaining if a count 
		// was given in argv.

	}

	free(pairArray);
	//Frees the node array once all sub pointers are freed.

}

int _freqCompar(const void *node1, const void *node2){
	struct Node *nodeOne = (struct Node *)node1;
	//Casts node1 as a pointer to a node in the form of nodeOne.

	struct Node *nodeTwo = (struct Node *)node2;
	//Casts node2 as a pointer to a node in the form of nodeTwo.

	return (nodeTwo->occuranceCount - nodeOne->occuranceCount);
	//Returns the difference in occurance between the two nodes.
}
