//This is part of the word pair program written by Jesse Jones.

#include "wordPairLinkedList.h"
#include"getWord.h"
#include<stdlib.h>
#include<string.h>

#define WORD_PAIR_MAX_SIZE 514
//This is the maximum number of bytes a word pair takes up.

char *generateWordPair(char *previousWord, char *currentWord){
	if (currentWord == NULL){
		return NULL;
	}
	//Null is returned if the second word 
	// is null since that is not a word pair.

	char *pairString = (char *)malloc(strlen(previousWord) + 1 + strlen(currentWord) + 1);
	//Creates a new string with room for word1, 
	// a space character, word2, and a null terminator.	

	char *space = " ";
	//Space character and null terminator used in concatenation.

	strncpy(pairString, previousWord, strnlen(previousWord, WORD_PAIR_MAX_SIZE) + 1);
	//Copies the first word of the pair 
	// to the first chunk of memory of the pairString.

	strncat(pairString, space, WORD_PAIR_MAX_SIZE);
	//Appends space character to word1 in pair string,
	// replacing the previous null terminator of the first word.

	strncat(pairString, currentWord, WORD_PAIR_MAX_SIZE);
	//Appends word2 to the space character, 
	// making the final word pair string, 
	// replacing the space string's null terminator.

	return pairString;

}


struct Node *createNode(char *pair, int occurNum, struct Node *next){
	if (pair == NULL || occurNum <= 0){
		return NULL;
	}
	//If there is no string pair to create a string with, 
	// or an unusual occurance number, return NULL.

	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	//Creates new blank node.

	if (newNode == NULL){
		return NULL;
	}
	//Returns NULL if malloc fails.

	newNode->occuranceCount = occurNum;
	newNode->next = next;
	//Sets number of occurances, next, and the pair string itself.

	char *pairStr = (char *)malloc(strnlen(pair, WORD_PAIR_MAX_SIZE) + 1);
	//Creates memory space the size of the input string.

	if (pairStr == NULL){
		free(newNode);
		return NULL;
	}
	//Frees the new node and returns null 
	// if the malloc for the pair string field fails.

	strcpy(pairStr, pair);
	//Copies value of input string pair to new pair string.

	(newNode->value) = pairStr;
	//Sets word pair field of node to the word pair inputted.

	return newNode;
}

struct LinkedList *createLinkedList(struct Node *headNode){
	struct LinkedList *newList = (struct LinkedList *)malloc(sizeof(struct LinkedList));
	//Creates pointer to new linked list.

	if (newList == NULL){
		return NULL;
	}
	//Handles case if malloc fails to get memory for the list.

	if (headNode == NULL){
		newList->size = 0;
	}else{
		newList->size = 1;
	}
	//Sets size of linked list to 0 if it's an empty list 
	// and one if it has a head.

	newList->head = headNode;
	//Sets head node of list.

	return newList;

}

int insertNode(struct LinkedList *LL, struct Node *insNode){
	if (insNode == NULL || LL == NULL){
		return 1;
	}
	//Returns 1 if there is no node to insert 
	// or linked list to insert into.

	if (LL->size == 0){
		LL->head = insNode;
		LL->size++;
		return 0;
	}
	//If list is empty, set head to insert node 
	// and increase size value of list.

	struct Node *cursor = LL->head;
	//Starts iteration cursor at the head of the linked list.

	while (1){
		int cmp = strncmp(insNode->value, cursor->value, WORD_PAIR_MAX_SIZE);
		//Compares the word pair of the inserting node 
		// with the word pair of the cursor.

		if (cmp == 0){
			cursor->occuranceCount++;
			return 2;
		}
		//If a matching word pair is found, the occurance count is updated, 
		// and the special 2 code is returned, 
		// indicating a non-erronious non-insertion.
		
		if (cursor->next == NULL){
			cursor->next = insNode;
			LL->size++;
			break;
		}
		//Inserts node into the open spot 
		// of the linked list and exits the loop.
		
		cursor = cursor->next;
		//Iterates to next node.
	}
	//Loops until node is either inserted or the occurance count is incremented.
	
	return 0;
}


int deleteNode(struct Node *delNode){
	if (delNode == NULL){
		return 1;
	}
	//There is no delNode to delete so the function exits with an error code.

	delNode->next = NULL;
	//Sets next to null to ensure it is detached from its former Linked List.

	free((delNode->value));
	//Frees the word pair string within the node.

	free(delNode);
	//Frees the delnode itself, deleting it.

	return 0;
}

int deleteLinkedList(struct LinkedList *delLL){
	if (delLL == NULL){
		return 1;
	}
	//If there is no linked list to delete, 1 is returned.

	struct Node *cursor = delLL->head;
	//Cursor starts at the head of the linked list.

	struct Node *temp;
	//Used in iterative deletion of linked list nodes.

	if (cursor == NULL){
		free(delLL);
		return 0;
	}
	//If there are no nodes in the Linked List, 
	// the empty linked list struct 
	// is immediately freed and 0 is returned.

	while (cursor != NULL){
		temp = cursor->next;
		deleteNode(cursor);
		cursor = temp;
	}
	//Iterates through linked list and frees the nodes.

	delLL->size = 0;
	delLL->head = NULL;
	//Sets size of the empty linked list to 0 
	// and sets the head to point to NULL.

	free(delLL);
	//Frees the linked list structure after it has been cleared.

	return 0;
}