#ifndef _LIST2_
#define _LIST2_


#include "athlete.h"
// basic struct for storing item in linked list (one direction)
struct list {
	struct list *next;
	struct athleteInfo *data;
};


struct list *nodeInsert(struct list *parent, struct athleteInfo *data);

void freeList(struct list *parent);

void printList(struct list *parent, FILE *fpt);

#endif