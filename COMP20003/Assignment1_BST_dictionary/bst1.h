#ifndef _BST1_
#define _BST1_


#include "athlete.h"


// bst structure has left and right
// stores a address to string for comparison purposes
// data is the information to be stored
struct bst {
    struct bst *left;
    struct bst *right;
    char *key;
    struct athleteInfo *data;
};

struct bst *bstInsert(struct bst *parent, struct athleteInfo *data);

void freeBst(struct bst *parent);

void searchBst(struct bst *parent, char *key,  FILE *fpt, int *m, long long *t);


#endif