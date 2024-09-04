//This is part of the word pair program written by Jesse Jones.

#ifndef WORDLINKLIST_H
#define WORDLINKLIST_H
//Handles double inclusion instances.

#include<stdio.h>

struct Node{
	//This struct is used as a node 
	// for a linked list to be contained within a hash table.

	int occuranceCount;
	//Tracks how many times the given value has occured in a file.

	struct Node *next;
	//Used for linked list iteration.

	void *value;
	//Contains the value of the node.
};

struct LinkedList{
	//This is a structure that contains all the nodes of a given linked list,
	// as well as some other attributes like its size 
	// and what the head node is.

	struct Node *head;
	//Pointer to head of linked list.

	int size;
	//Tracks the size of linked list.	
};

char *generateWordPair(char *previousWord, char *currentWord);
//Generates a word pair string with previous word followed by a space, 
// followed by currentWord, ending in a null terminator. 
// This is effectively a string concatenator 
// that returns a pair of strings separated by a space.

//Returns NULL if word pair could not be generated. 

struct Node *createNode(char *pair, int occurNum, struct Node *next);
//This function does all the memory work necessary 
// to create a node for a given linked list 
// with information from the arguments provided.

//Returns NULL if node could not be created.

struct LinkedList *createLinkedList(struct Node *headNode);
//This function creates a brand new linked list 
// with a size of 1 if the headNode provided is not null.

//Returns NULL if linked list coult not be made.

int insertNode(struct LinkedList *LL, struct Node *insNode);
//This function inserts the input node into the input linked list (LL).

//Returns 1 if insertion to linked list failed. 
//Returns 2 if there is already a node 
// with the insNode's value in the linked list.

int deleteNode(struct Node *delNode);
//This function deletes a node by freeing all 
// of its subcomponents and then freeing the node itself.
//This is useful in the deletion of the whole hash table.

//Returns 1 if deletion of node failed.

int deleteLinkedList(struct LinkedList *delLL);
//Deletes a linked list passed into it.

//Returns 1 if linked list deletion failed.




#endif