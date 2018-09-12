#ifndef _BST2_
#define _BST2_


#include "athlete.h"

// added an extra list variable in struct to store same name item as list
struct bst {
    struct bst *left;
    struct bst *right;
    struct list *next;
    char *key;
    struct athleteInfo *data;
};


struct bst *bstInsert(struct bst *parent, struct athleteInfo *data);

void freeBst(struct bst *parent);

void searchBst(struct bst *parent, char *key,  FILE *fpt, int *m, long long *t);

#endif