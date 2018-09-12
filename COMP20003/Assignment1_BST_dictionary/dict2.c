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

#include "bst2.h"
#include "athlete.h"

#define FALSE 0
#define TRUE 1

#define BUFFER_SIZE 512
long long compT = 0;
int compM = 0;
// main
int main(int argc, char **argv){
	
	// arg[0]: ./program
	// arg[1]: datafile to be read
	// arg[2]: outputfile name (include type)
	// < keyfile: search for these
	
	// opening and reading the file specified by the 2nd argument
	FILE *fptr = fopen(argv[1], "r");
	// For storing a line of text from csv
	char buffer[BUFFER_SIZE + 1];
	// The root node
	struct bst *bst2 = NULL;
	
	
	
	// for checking the creating and inserting of the tree
    clock_t start, end;
    double cpu_time_used;
    // get the time before the iteration
	start = clock();
	 while (fgets(buffer, BUFFER_SIZE+1, fptr) != NULL){
	 	if (buffer[strlen(buffer)-1] == '\n'){
			buffer[strlen(buffer)-1] = '\0';
		}
    	bst2 = bstInsert(bst2, createInfo(buffer));
    }
    
    /**
    * Back up method if fgets doesn't work
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
				bst2 = bstInsert(bst2, createInfo(buffer));
				isStartingNewLine = TRUE;
			} else {
				isStartingNewLine = FALSE;
			}
  	}*/
  	// get the time afer iteration ended
  	end = clock();
  	// close the file that was opened for inserting the bst tree
  	fclose(fptr);
  	
  	// get the time in seconds
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
  	printf("Insertion took %f seconds to execute \n", cpu_time_used);
  	
  	
  	// open another file for writing the search result
	fptr = fopen(argv[2],"w");
  	
	// Storing each line from the keyfile, know name cannot exceed 128 char
	char keyBuffer[STRING_MAX+1];
	start = clock();
	while (fgets(keyBuffer, STRING_MAX+1, stdin)!=NULL){
		if (keyBuffer[strlen(keyBuffer)-1] == '\n'){
			keyBuffer[strlen(keyBuffer)-1] = '\0';
		}
		searchBst(bst2, keyBuffer, fptr, &compM, &compT);
	}
	/**
	* Back up method if fgets doesn't work
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
			searchBst(bst2, keyBuffer, fptr);
			isStartingNewLine = TRUE;
		} else {
			isStartingNewLine = FALSE;
		}
	}*/
	end = clock();
	fclose(fptr);       
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  	printf("Search took %f seconds to execute \n", cpu_time_used);
  	printf("Average comparison: %.3f\n", 1.0*compT/compM);
	freeBst(bst2);
	
	return 0;
}