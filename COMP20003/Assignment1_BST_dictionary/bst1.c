/**
* COMP20003 Assignment 1
* 2018 Sem 2
* Yang Zhou
* References:
* bst
* 	comp20003 workshop 4
* Writing in a file
*	https://www.geeksforgeeks.org/fprintf-in-c/
* freeing tree
* 	Comp20003 lecture material on bst traversal
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "athlete.h"
#include "bst1.h"

/**
* inserts athlete information into a bst tree from the root
* parent: the root node
* data: the address of the athlete created
* @return: the parent address
*/
struct bst *bstInsert(struct bst *parent, struct athleteInfo *data){
	struct bst **insertLocation = &parent;
	int diff;
	
	while (*insertLocation){
		// do some comparison if less than equal to name go left otherwise right
		diff = strcmp(data->name, (*insertLocation)->key);
		if (diff <= 0){                                              
			insertLocation = &((*insertLocation)->left);
		} else {
			insertLocation = &((*insertLocation)->right);
		}
	}
	*insertLocation = (struct bst *)malloc(sizeof(struct bst));
	(*insertLocation)->left = (*insertLocation)->right = NULL;
	// Since name already allocated space just make key point to same address
	(*insertLocation)->key = data->name;
	(*insertLocation)->data = data;
	
	return parent;
}

/**
* frees memory allocated for the bst root node and its children
* parent: the address of parent node
*/
void freeBst(struct bst *parent){
	// recursive method
	// if not null go in
	if (parent){
		// make sure node is the last one in the structure
		// go leftmost first
		freeBst(parent->left);
		// after go right most
		freeBst(parent->right);
		
		// use helper to free the information assigned to bst node
		freeAthleteInfo(parent->data);
		// free this node
		free(parent);
	}
}
/**
* Searches a bst for a name and print out the associated result in file
* This implementation uses list structure at bst nodes to store same name data
* parent: the root bst node address
* key: the name string to be searched
* fpt: file pointer that is to be written in
*/
void searchBst(struct bst *parent, char *key,  FILE *fpt, int *m, long long *t){
	struct bst **pos = &parent;
	// the difference between 2 strings
	int diff;
	// number of comparisons made
	int comparisons = 0;
	int found = 0;
	*m += 1;
	while (*pos){
		diff = strcmp(key, (*pos)->key);
		comparisons++;
		if (diff == 0){
			printAthlete((*pos)->data, fpt);
			found++;
		}
		if (diff <= 0){
			pos = &((*pos)->left);
		} else {
			pos = &((*pos)->right);
		}
	}
	*t+=comparisons;
	if (found == 0){
		fprintf(fpt, "%s --> NOTFOUND\n", key);
	}
	printf("%s --> %d\n", key, comparisons);
}