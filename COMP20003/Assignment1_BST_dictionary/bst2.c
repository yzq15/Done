/**
* COMP20003 Assignment 1
* 2018 Sem 2
* Yang Zhou
* References:
* bst
* 	comp20003 workshop 3 and 4
* Writing in a file
*	https://www.geeksforgeeks.org/fprintf-in-c/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "athlete.h"
#include "bst2.h"
#include "list2.h"
/**
* insert athlete info into a bst
* parent: the root node to insert the data into
* data: athlete address that has information to be stored
* @return: parent node address
*/
struct bst *bstInsert(struct bst *parent, struct athleteInfo *data){
	struct bst **insertLocation = &parent;
	int diff;
	
	// flag to determine whether storing a list item or bst item
	int inList = 0;
	// Same implemention as before only difference is that list is created
	// at the node that has the same key
	while (*insertLocation){
		diff = strcmp(data->name, (*insertLocation)->key);
		if (diff < 0){                                              
			insertLocation = &((*insertLocation)->left);
		} else if (diff > 0){
			insertLocation = &((*insertLocation)->right);
		} else {
			inList = 1;
			// break to exit the iteration of the bst tree
			break;
		}
	}
	
	if (inList){
		// use helper function to store the athlete in the list nodes
		(*insertLocation)->next = nodeInsert((*insertLocation)->next, data);
	} else {
		*insertLocation = (struct bst *)malloc(sizeof(struct bst));
		(*insertLocation)->left = NULL;
		(*insertLocation)->right = NULL;
		(*insertLocation)->next = NULL;
		// Use already stored name as the address for bst key
		(*insertLocation)->key = data->name;
		(*insertLocation)->data = data;
	}
	return parent;
}

/**
* Recursive method to free the parent node
* parent: the address to parent node
*/
void freeBst(struct bst *parent){
	if (parent){
		freeBst(parent->left);
		freeBst(parent->right);
		// need to free the list (athlete infos) that was created at a bst node
		freeList(parent->next);
		// free the bst node that contains the athlete info
		freeAthleteInfo(parent->data);
		// free the parent node
		free(parent);
	}
}
/**
* searches a name(key) in bst and print out the result in a file indicated by
* pointer
* parent: the bst root node address
* key: the string of a key to search for in tree
* fpt: a file pointer 
*/
void searchBst(struct bst *parent, char *key,  FILE *fpt, int *m, long long *t){
	struct bst **pos = &parent;
	int diff;
	// Comparisons made
	int comparisons = 0;
	// Items found, printed out in the stdout
	int found = 0;
	*m += 1;
	while (*pos){
		diff = strcmp(key, (*pos)->key);
		comparisons++;
		
		if (diff < 0){
			pos = &((*pos)->left);
		} else if (diff > 0){
			pos = &((*pos)->right);
		} else {
			printAthlete((*pos)->data, fpt);
			printList((*pos)->next, fpt);
			found++;
			// to exit the iteration
			break;
		}
		
	}
	*t += comparisons;
	if (found == 0){
		fprintf(fpt, "%s --> NOTFOUND\n", key);
	}
	printf("%s --> %d\n", key, comparisons);
}