/**
* COMP20003 Assignment 1
* 2018 Sem 2
* Yang Zhou
* References:
* inserting into list
*	comp20003 workshop 3 and 4 solution
*/

#include <stdlib.h>
#include <stdio.h>
#include "list2.h"
#include "athlete.h"

/**
* store athlete information in a list originated from the parent list (node)
* data: info address to be stored
* parent: the head of the list
* @return: the parent address
*/
struct list *nodeInsert(struct list *parent, struct athleteInfo *data){
	struct list **insertLocation = &parent;
	// Same implementation as the bst insert but no comparison just check
	// wheter address is null and insert
	while (*insertLocation){                                            
		insertLocation = &((*insertLocation)->next);
	}
	
	*insertLocation = (struct list *)malloc(sizeof(struct list));
	(*insertLocation)->next = NULL;
	(*insertLocation)->data = data;
	
	return parent;
}
/**
* Free a list given its head
*/
void freeList(struct list *parent){
	// recursive implementation
	if (parent){
		freeList(parent->next);
		freeAthleteInfo(parent->data);
		free(parent);
	}
}

/**
* prints the items in a list in a specific form into a file
* parent: the head or parent node that starts the list
* fpt: the file to be printed into
*/
void printList(struct list *parent, FILE *fpt){
	if (parent){
		printAthlete(parent->data, fpt);
		printList(parent->next, fpt);
	}
}