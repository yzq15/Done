/**
* COMP20003 Assignment 1
* 2018 Sem 2
* Yang Zhou
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "athlete.h"

/**
* Takes in a string that has 15 fields seperated by "," and create athlete info
* buffer: string that contains athlete information
* @return: memory address to the struct created for an athlete 
*/
struct athleteInfo *createInfo(char *buffer){
	struct athleteInfo *info = malloc(sizeof(struct athleteInfo));
	
	// Column is the athlete field that we're currently parsing
	int column = 0;
	int bufferLen = strlen(buffer);
	
	// attLen for counting the number of characters for a field
	int i, attLen = 0;
	// If in " " don't count the ","
	int inApo = FALSE;
	// For temorarily storing a field string
	char temp[STRING_MAX + 1];
	for (i = 0; i <= bufferLen; i++){

		if (buffer[i] == '"' && inApo == FALSE){
			inApo = TRUE;
		} else if (buffer[i] == '"' && inApo == TRUE){
			inApo = FALSE;
		}
		// Store the character in temp if not "," and "\0"
		if (buffer[i] != ',' && inApo == 0 && buffer[i] != '\0'){
			temp[attLen++] = buffer[i];
		} else if (inApo == TRUE){
			// When in " ", don't break the field continue storing
			temp[attLen++] = buffer[i];
		} else {
			temp[attLen] = '\0';
		
			/*
			// (void*)info + column*sizeof(char*) will be the address to the 
			// variables storing address to strings, so to effectively to access
			// need to use char** as a type cast, one * for being the address
			// ** being the exact location of the string stored
			
			Does not work on server, this function tries to go to address of
			struct and goes to each variable by memory location and assign 
			address to string memory and then copy
			
			((char**)((void*)info + column*sizeof(char*))) = 
			malloc(sizeof(char)*(attLen+1));
			strcpy(*((char**)((void*)info + column*sizeof(char*))),temp);
			*/
			switch (column){
			case 0:
				info->id= malloc(sizeof(char)*(attLen+1));
				strcpy(info->id, temp);
				break;
			
			case 1:
				info->name = malloc(sizeof(char)*(attLen+1));
				strcpy(info->name , temp);
				break;
			
			case 2:
				info->sex = malloc(sizeof(char)*(attLen+1));
				strcpy(info->sex , temp);
				break;
			
			case 3:
				info->age = malloc(sizeof(char)*(attLen+1));
				strcpy(info->age , temp);
				break;
			
			case 4:
				info->height = malloc(sizeof(char)*(attLen+1));
				strcpy(info->height , temp);
				break;
			
			case 5:
				info->weight = malloc(sizeof(char)*(attLen+1));
				strcpy(info->weight , temp);
				break;
			
			case 6:
				info->team = malloc(sizeof(char)*(attLen+1));
				strcpy(info->team , temp);
				break;
			
			case 7:
				info->noc = malloc(sizeof(char)*(attLen+1));
				strcpy(info->noc , temp);
				break;
			
			case 8:
				info->games = malloc(sizeof(char)*(attLen+1));
				strcpy(info->games , temp);
				break;
			
			case 9:
				info->year = malloc(sizeof(char)*(attLen+1));
				strcpy(info->year , temp);
				break;
			
			case 10:
				info->season = malloc(sizeof(char)*(attLen+1));
				strcpy(info->season , temp);
				break;
			
			case 11:
				info->city = malloc(sizeof(char)*(attLen+1));
				strcpy(info->city , temp);
				break;
			
			case 12:
				info->sport = malloc(sizeof(char)*(attLen+1));
				strcpy(info->sport , temp);
				break;
			
			case 13:
				info->event = malloc(sizeof(char)*(attLen+1));
				strcpy(info->event , temp);
				break;
			
			case 14:
				info->medal = malloc(sizeof(char)*(attLen+1));
				strcpy(info->medal , temp);
				break;
			}
			
			
			column++;
			attLen = 0;
		}
	}
	return info;
}

/**
* free the athlete addresses that was created
* info: the address of the athlete
*/
void freeAthleteInfo(struct athleteInfo *info){
	int i;
	// free each field that was assigned memory address
	for(i = 0; i <= TOTAL_FIELDS; i++){
		free(*((char**)((void*)info + i*sizeof(char*))));
	}
	// free the struct that was created to store athlete
	free(info);
}
/**
* Prints the information of athlete in a specific format in a file
* info: the athlete pointer to print
* fpt: the file pointer to where to print the results to
*/
void printAthlete(struct athleteInfo *info, FILE *fpt){
	/*
	a --> ID: 9 Sex: F || Age: 5 || Height: 6 || Weight: 8 
	|| Team: T || NOC: AUS || Games: 1 Summer || Year: 7 
	|| Season: Summer || City: A || Sport: X || Event: Y 
	|| Medal: NA ||
	*/
	fprintf(fpt, "%s --> ", info->name);
	fprintf(fpt, "ID: %s Sex: %s || ", info->id, info->sex);
	fprintf(fpt, "Age: %s || ", info->age);
	fprintf(fpt, "Height: %s || ", info->height);
	fprintf(fpt, "Weight: %s || ", info->weight);
	fprintf(fpt, "Team: %s || ", info->team);
	fprintf(fpt, "NOC: %s || ", info->noc);
	fprintf(fpt, "Games: %s || ", info->games);
	fprintf(fpt, "Year: %s || ", info->year);
	fprintf(fpt, "Season: %s || ", info->season);
	fprintf(fpt, "City: %s || ", info->city);
	fprintf(fpt, "Sport: %s || ", info->sport);
	fprintf(fpt, "Event: %s || ", info->event);
	fprintf(fpt, "Medal: %s ||\n", info->medal);
}                                                   