/**
* COMP20003 Assignment 1
* 2018 Sem 2
* Yang Zhou
* References:
* Measuring time of method
* 	https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/
* Reading lines from file and input stream
* 	https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm
	https://stackoverflow.com/questions/3919009/how-to-read-from-stdin-with-fgets
*/

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "bst1.h"
#include "athlete.h"

#define FALSE 0
#define TRUE 1

#define BUFFER_SIZE 512

long long compT = 0;
int compM = 0;

// Prototypes
void printAscending(struct bst *parent, FILE *fptr);

// main
int main(int argc, char **argv){
	
	// arg[0]: ./program
	// arg[1]: datafile to be read
	// arg[2]: outputfile name (include type)
	// < keyfile: search for these
	
	// opening and reading the file specified by the 2nd argument
	FILE *fptr = fopen(argv[1], "r");
	
	char buffer[BUFFER_SIZE + 1];
	struct bst *bst1 = NULL;
	
	
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    while (fgets(buffer, BUFFER_SIZE+1, fptr) != NULL){
    	if (buffer[strlen(buffer)-1] == '\n'){
			buffer[strlen(buffer)-1] = '\0';
		}
    	bst1 = bstInsert(bst1, createInfo(buffer));
    }
    /**
    // If fgets does not work 
    int isStartingNewLine = TRUE;
	int ch;
	int linePos;
  	while( (ch = getc(fptr)) != EOF ) {

			if (isStartingNewLine){
				linePos = 0;
			}
			if (ch != '\n') {
				buffer[linePos++] = ch;
			}
			if (ch == '\n'){
				buffer[linePos] = '\0';
				bst1 = bstInsert(bst1, createInfo(buffer));
				isStartingNewLine = TRUE;
			} else {
				isStartingNewLine = FALSE;
			}
  	}*/
  	fclose(fptr);
  	end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  	printf("Insertion took %f minute to execute \n", cpu_time_used/60.0);
    
	fptr = fopen(argv[2],"w");
	char keyBuffer[STRING_MAX+1];
	start = clock();
	while (fgets(keyBuffer, STRING_MAX+1, stdin)){
		if (keyBuffer[strlen(keyBuffer)-1] == '\n'){
			keyBuffer[strlen(keyBuffer)-1] = '\0';
		}
		searchBst(bst1, keyBuffer, fptr, &compM, &compT);
	}
	end = clock();
	/**
	// If fgets does not work
	isStartingNewLine = TRUE;
	while((ch = getchar()) != EOF){
		if (isStartingNewLine){
			linePos = 0;
		}
		
		if (ch != '\n'){
			keyBuffer[linePos++] = ch;
		}
		
		if (ch == '\n'){
			keyBuffer[linePos] = '\0';
			searchBst(bst1, keyBuffer, fptr);
			isStartingNewLine = TRUE;
		} else {
			isStartingNewLine = FALSE;
		}
	}*/
	fclose(fptr);
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  	printf("Search took %f seconds to execute \n", cpu_time_used);
  	printf("Average comparison: %.3f\n", 1.0*compT/compM);
	freeBst(bst1);
	return 0;
}

// For printing the field in bst from ascending order according to name
void printAscending(struct bst *parent, FILE *fptr){
	if(parent){
		printAscending(parent->left, fptr);
		fprintf(fptr,"%s\n",parent->data->name);
		printAscending(parent->right,fptr);
	}
	/*
	fptr = fopen("athleteNames.txt", "w");
	printAscending(bst1, fptr);
	fclose(fptr);
	*/
}