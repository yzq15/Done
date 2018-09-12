#ifndef _ATHLETE_
#define _ATHLETE_


#define STRING_MAX 128
#define TRUE 1
#define FALSE 0
#define TOTAL_FIELDS 14

// athlete information in a struct
struct athleteInfo {
	char *id;
	char *name;
	char *sex;
	char *age;
	char *height;
	char *weight;
	char *team;
	char *noc;
	char *games;
	char *year;
	char *season;
	char *city;
	char *sport;
	char *event;
	char *medal;
};

struct athleteInfo *createInfo(char *buffer);

void freeAthleteInfo(struct athleteInfo *info);

void printAthlete(struct athleteInfo *info, FILE *fpt);

#endif