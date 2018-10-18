/*
 * COMP20003 Algorithms and Data Structures Second (Spring) Semester 2018
 * [Assignment 2]
 * Solving Puzzle Games Optimally: Graph Search
 * Yang Zhou
 * 693507
 * Modified
 * apply(), ida(), IDA_control_loop(), manhattan(), struct node
 * Added
 * sameColLinConf(), sameRowLinConf(), localLinearConflict()
 * Programming is fun!!!
 */

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
	int g;
	int f;
	// Storing the reverse op
	// ensure that not traverse back when in recursion
	int reverse;
} node;

/**
 * Global Variables
 */

// used to track the position of the blank in a state,
// so it doesn't have to be searched every time we check if an operator is applicable
// When we apply an operator, blank_pos is updated
int blank_pos;

// used to track the previous move, this will ensure that a change in state
// is not changed back
//int previous;

// Initial node of the problem
node initial_node;

// Statistics about the number of generated and expendad nodes
unsigned long generated;
unsigned long expanded;


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

// faster retrieval for reverse action
int ap_reverse[] = {RIGHT, LEFT, DOWN, UP};

// prototype for method all the way at the bottom
int sameColLinConf(int* state, int tile, int to);
int sameRowLinConf(int* state, int tile, int to);
int localLinearConflict(int* state, int tile, int op);

/* print state */
void print_state( int* s )
{
	int i;

	for( i = 0; i < 16; i++ )
		printf( "%2d%c", s[i], ((i+1) % 4 == 0 ? '\n' : ' ') );
}

void printf_comma (long unsigned int n) {
    if (n < 0) {
        printf ("-");
        printf_comma (-n);
        return;
    }
    if (n < 1000) {
        printf ("%lu", n);
        return;
    }
    printf_comma (n/1000);
    printf (",%03lu", n%1000);
}

/* Implemented Code */
/* return the sum of manhattan distances from state to goal */
int manhattan( int* state )
{
    int sum = 0;
    // des is the tile destination positoin
	// cur is the tile current position
	int i, j, x_des, y_des, x_cur, y_cur;
	for (i = 0; i < PUZZLE_SIZE; i++){
        // 0 tile is not considered for the manhattan distance
		if (state[i] != 0){
			x_des = state[i] % SIDE_LEN;
			y_des = state[i] / SIDE_LEN;
			x_cur = i % SIDE_LEN;
			y_cur = i / SIDE_LEN;
			sum += abs(x_cur - x_des) + abs(y_cur - y_des);
		}
		// does the linear conflict calculation for tile with rest of row
		for (j = 1; (i+j) % SIDE_LEN != 0; j++){
			if (sameRowLinConf(state, i, i+j)){
				sum+=2;
			}
		}
		// does the linear conflict calculation for tile with rest of column
		for (j = 4; (i+j) < PUZZLE_SIZE; j+=4){
			if (sameColLinConf(state, i, i+j)){
				sum+=2;
			}
		}
	}


	return( sum );
}


/* return 1 if op is applicable in state, otherwise return 0 */
int applicable( int op )
{
       	return( ap_ops[op][blank_pos] );
}

/* Changes made: Does calculations for net manhattan difference after apply */
/* apply operator */
int apply( node* n, int op )
{
	int t;
	int net = 0;
	//find tile that has to be moved given the op and blank_pos
	t = blank_pos + (op == 0 ? -1 : (op == 1 ? 1 : (op == 2 ? -4 : 4)));

	/* Customisation starts here */
	// Finds the net of the manhattan difference from one node to another
	// not do calculation if the apply is to reverse the node
	if (op != initial_node.reverse){
		int x_cur, y_cur, x_des, y_des, xb_cur, yb_cur;

		// the tile position for destination
		x_des = initial_node.state[t] % SIDE_LEN;
		y_des = initial_node.state[t] / SIDE_LEN;

		// tile current position
		x_cur = t % SIDE_LEN;
		y_cur = t / SIDE_LEN;

		// the position for blank
		// hypothetically if the tile is moved here what is the distance
		xb_cur = blank_pos % SIDE_LEN;
		yb_cur = blank_pos / SIDE_LEN;

		/* Find the difference (net) between if moved tile is
		 * in blank and current tile distance (manhattan uses the sum)
		 */
		net = abs(xb_cur - x_des) + abs(yb_cur - y_des)
					- (abs(x_cur - x_des)+abs(y_cur - y_des));
	}


    // finds the linear conflict for t-tile in row and column before change
	// not do calculation if apply is to reverse state
	int before = 0;
	if (op != initial_node.reverse){
		before = localLinearConflict(n->state, t, op);
	}


	//apply op
	n->state[blank_pos] = n->state[t];
	n->state[t] = 0;

    // New linear conflict contributed from t-tile added to new position
	// not do calculation if apply is to reverse state
	int after =0;
	if (op != initial_node.reverse){
		after = localLinearConflict(n->state, blank_pos, op);
	}

    net += (after - before);

    /* Customisation ends here */
	// store the reverse here
	// This will ensure that we don't do move or reverse with this
	n->reverse = ap_reverse[op];
	//update blank pos
	blank_pos = t;
	return net;
}


/* Recursive IDA */
node* ida( node* node, int threshold, int* newThreshold)
{
	struct node * r = NULL;

	/*Pseudocode Starts here*/
	// index for operation applicable
	int op_i;

	// Storing the manhattan will complete iteration faster
	// f is equivalent to n.g + manhattan
	int curManhattan = node->f - node->g; //manhattan(node->state);

	// Used for calculation purposes
	int newManhattan = curManhattan;

	// Store the reverse to function method reverse (stack memory)
	// to ensure that after recursion changes it, it is set back
	// also reduces the amount of step through when checking apply move
	int reverse = node->reverse;

	for (op_i = 0; op_i < OP_COUNT; op_i++){
		// reverse and applicable is used as the guard
		if (applicable(op_i) == 1 && op_i != reverse){
			generated++;

			// adding the net manhattan (next state - previous state)
			newManhattan += apply(node, op_i);
			/* all pseudo */
			node->g += 1;
			node->f = node->g + newManhattan;
			if (node->f > threshold){
				*newThreshold =
					(node->f < *newThreshold) ? node->f : *newThreshold;
			} else {
				expanded++;
				if (newManhattan == 0){
					return node;
				}
				r = ida(node, threshold, newThreshold);
				if (r != NULL){
					return r;
				}
			}
			//reverse apply() to get to the state before move for next iteration
			apply(node, node->reverse);
			node->g -= 1;

			// changing back to the previous manhattan state for next operation
 			newManhattan = curManhattan;
		}
	}

	// set back n.f (important becuase it is the manhattan used for calculation)
	// outside loop because it constantly manipulated inside the loop
	// previous n.f
	node->f = curManhattan + node->g;
    // same reason as node->f using the stack frame to store the reverse
    node->reverse = reverse;
	/*Pseudocode ends here*/

	return( NULL );
}


/* main IDA control loop */
int IDA_control_loop(  ){
	node* r = NULL;

	int threshold, newThreshold;

	/* initialize statistics */
	generated = 0;
	expanded = 0;

	/* compute initial threshold B */
	initial_node.f = threshold = manhattan( initial_node.state );

	/* Pseudocode starts here*/

	// Set reverse to previous to -1 meaning that there is no reverse move
	initial_node.reverse = -1;
	printf( "Initial Estimate = %d\nThreshold = ", threshold );

	while (r == NULL){
		printf("%d ", threshold);
		newThreshold = INT_MAX;
		r = ida(&initial_node, threshold, &newThreshold);
		if (r == NULL){
			threshold = newThreshold;
		}
	}
	/* Pseudocode ends here */

	if(r != NULL)
		return r->g;
	else
		return -1;
}


static inline float compute_current_time()
{
	struct rusage r_usage;

	getrusage( RUSAGE_SELF, &r_usage );
	float diff_time = (float) r_usage.ru_utime.tv_sec;
	diff_time += (float) r_usage.ru_stime.tv_sec;
	diff_time += (float) r_usage.ru_utime.tv_usec / (float)1000000;
	diff_time += (float) r_usage.ru_stime.tv_usec / (float)1000000;
	return diff_time;
}

int main( int argc, char **argv )
{
	int i, solution_length;

	/* check we have a initial state as parameter */
	if( argc != 2 )
	{
		fprintf( stderr, "usage: %s \"<initial-state-file>\"\n", argv[0] );
		return( -1 );
	}


	/* read initial state */
	FILE* initFile = fopen( argv[1], "r" );
	char buffer[256];

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

	/* initialize the initial node */
	initial_node.g=0;
	initial_node.f=0;

	print_state( initial_node.state );


	/* solve */
	float t0 = compute_current_time();

	solution_length = IDA_control_loop();

	float tf = compute_current_time();

	/* report results */
	printf( "\nSolution = %d\n", solution_length);
	printf( "Generated = ");
	printf_comma(generated);
	printf("\nExpanded = ");
	printf_comma(expanded);
	printf( "\nTime (seconds) = %.2f\nExpanded/Second = ", tf-t0 );
	printf_comma((unsigned long int) expanded/(tf+0.00000001-t0));
	printf("\n\n");

	/* aggregate all executions in a file named report.dat, for marking purposes */
	FILE* report = fopen( "report.dat", "a" );

	fprintf( report, "%s", argv[1] );
	fprintf( report, "\n\tSoulution = %d, Generated = %lu, Expanded = %lu", solution_length, generated, expanded);
	fprintf( report, ", Time = %f, Expanded/Second = %f\n\n", tf-t0, (float)expanded/(tf-t0));
	fclose(report);

	return( 0 );
}

/* finds the Linear conflict for one tile in its row and column */
int localLinearConflict(int* state, int tile, int op){
	int sum = 0;
	int i,j;
	// The idea is to find the linear conflict contributed in its row and column

    // uses op to keep track which action if same row or column as before don't
    // recalculate the linear conflict

    // check the linear conflict for column of the tile being moved or moved into
	for (i=4, j=4; i+j <= PUZZLE_SIZE && op != UP && op != DOWN;){
		if ((tile - i) >= 0){
			if (sameColLinConf(state, tile, tile-i)){
				sum+=2;
			}
			i+=4;
		}
		if ((tile +j) < PUZZLE_SIZE){
			if (sameColLinConf(state, tile, tile+j)){
				sum+=2;
			}
			j+=4;
		}
	}


    // check the linear conflict for row of the tile being moved or moved into
    for (i=1, j=1; i+j <= SIDE_LEN && op != LEFT && op != RIGHT;){
    	if ((tile-i+1)%SIDE_LEN != 0){
    		if (sameRowLinConf(state, tile, tile-i)){
    			sum+=2;
    		}
		    i++;
    	}
    	if ((tile+j)%SIDE_LEN != 0){
    		if (sameRowLinConf(state, tile, tile+j)){
    			sum+=2;
    		}
    		j++;
    	}
    }
	return sum;
}
/* checks if a tile and another compared tile is in row linear conflict */
// return 1 if in conflict 0 otherwise
int sameRowLinConf(int* state, int tile, int to){
	if (state[to] == 0){
		return 0;
	}
	if (state[tile] / SIDE_LEN == state[to] / SIDE_LEN
		&& tile / SIDE_LEN == state[tile] / SIDE_LEN) {

		if (state[tile] > state[to]){
			if (tile < to){
				return 1;
			}
		} else {
			if (to < tile){
				return 1;
			}
		}
	}
	return 0;
}

/* checks if a tile and another compared tile is in column linear conflict */
// return 1 if in conflict 0 otherwise
int sameColLinConf(int* state, int tile, int to){
	if (state[to] == 0){
		return 0;
	}
	if (state[tile] % SIDE_LEN == state[to] % SIDE_LEN
		&& tile % SIDE_LEN == state[tile] % SIDE_LEN){

		if (state[tile] > state[to]){
			if (tile < to){
				return 1;
			}
		} else {
			if (to < tile){
				return 1;
			}
		}
	}
	return 0;
}
