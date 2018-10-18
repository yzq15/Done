#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>


/**
 * READ THIS DESCRIPTION
 *
 * node data structure: containing state, g, f
 * you can extend it with more information if needed
 */
typedef struct node{
	int state[16];
} node;

/**
 * Global Variables
 */

// used to track the position of the blank in a state,
// so it doesn't have to be searched every time we check if an operator is applicable
// When we apply an operator, blank_pos is updated
int blank_pos;

// Initial node of the problem
node initial_node;


/**
 * The id of the four available actions for moving the blank (empty slot). e.x.
 * Left: moves the blank to the left, etc.
 */

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define SIDE_LEN 4
#define OP_COUNT 4
#define PUZZLE_SIZE 16

/*
 * Helper arrays for the applicable function
 * applicability of operators: 0 = left, 1 = right, 2 = up, 3 = down
 */
int ap_opLeft[]  = { 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 };
int ap_opRight[]  = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
int ap_opUp[]  = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
int ap_opDown[]  = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
int *ap_ops[] = { ap_opLeft, ap_opRight, ap_opUp, ap_opDown };


/* print state */
void print_state( int* s )
{
	int i;

	for( i = 0; i < 16; i++ )
		printf( "%2d%c", s[i], ((i+1) % 4 == 0 ? '\n' : ' ') );
}



/* return 1 if op is applicable in state, otherwise return 0 */
int applicable( int op )
{
       	return( ap_ops[op][blank_pos] );
}


/* apply operator */
void apply( node* n, int op )
{
	int t;

	//find tile that has to be moved given the op and blank_pos
	t = blank_pos + (op == 0 ? -1 : (op == 1 ? 1 : (op == 2 ? -4 : 4)));

	//apply op
	n->state[blank_pos] = n->state[t];
	n->state[t] = 0;

	//update blank pos
	blank_pos = t;
}

/* Recursive IDA */
int ida( node* node, char* outfptr)
{
	struct node * r = NULL;
	int op_i;
    char op_c;
    int i;
    scanf("%c", &op_c);
    getchar(); // read newline
    if (op_c == 'l'){
        op_i = LEFT;
    } else if (op_c == 'r'){
        op_i = RIGHT;
    } else if (op_c == 'u'){
        op_i = UP;
    } else if (op_c == 'd'){
        op_i = DOWN;
    } else {return -1;}
    if (applicable(op_i)){
        apply(node, op_i);
        FILE* out = fopen(outfptr, "w");
        fprintf(out, "%d", (node->state)[0]);
        for (i = 1; i < PUZZLE_SIZE; i++){
            fprintf(out, " %d", (node->state)[i]);
        }
		fclose(out);
    } else {
        printf("invalid move\n");
    }
	return 0;
}

int d = 0;
/* main IDA control loop */
int IDA_control_loop(char* outfptr){
	int r = 0;

	while (r == 0){
        print_state(initial_node.state);
        printf("\n");
		r = ida(&initial_node, outfptr);
	}

	if(r)
        return 1;
	else
		return -1;
}


int main( int argc, char **argv )
{
	int i, solution_length;

	/* check we have a initial state as parameter */
	if( argc != 3 )
	{
		fprintf( stderr, "usage: %s \"<initial-state-file> <output-file>\"\n", argv[0] );
		return( -1 );
	}


	/* read initial state */
	FILE* initFile = fopen( argv[1], "r" );
	char buffer[256];

    /* write to file */
    char* outfptr = argv[2];

	if( fgets(buffer, sizeof(buffer), initFile) != NULL ){
		char* tile = strtok( buffer, " " );
		for( i = 0; tile != NULL; ++i )
			{
				initial_node.state[i] = atoi( tile );
				blank_pos = (initial_node.state[i] == 0 ? i : blank_pos);
				tile = strtok( NULL, " " );
			}
	}
	else{
		fprintf( stderr, "Filename empty\"\n" );
		return( -2 );

	}

	if( i != 16 )
	{
		fprintf( stderr, "invalid initial state\n" );
		return( -1 );
	}


	/* solve */
    printf("move with u, d, l, r\n");
	solution_length = IDA_control_loop(outfptr);

	/* report results */
	printf("\n\n");

	return( 0 );
}
