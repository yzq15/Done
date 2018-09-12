/**
* COMP20005 Assignment 1
* 2018 Sem 2
* Yang Zhou (693507)
* C programming is fun!!!
*/
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_STRLEN 128
#define MAX_POINT 50
#define PI_RAD 3.14159
#define PI_DEG 180
#define EARTH_R 6371000
#define INTERVAL 30
#define ID_LEN 10
#define DATE_ITEM 3
#define TIME_ITEM 3
#define POS_ITEM 2

void readLocInfo(char *line, int date[], int time_[], double pos[], char id[]);
double distance(double pos1[], double pos2[]);
double calculateSpeed(double dist, int seconds);
double to_radian(double x);

int main(int argc, char **argv){
    // number of char in line read
    int len;
    char buffer[MAX_POINT][MAX_STRLEN];
    // buddy that counts the number of lines read
    int read = 0;

    while(fgets(buffer[read], MAX_STRLEN, stdin)){
        len = strlen(buffer[read]);
        if (buffer[read][len - 1] == '\n'){
            buffer[read][len - 1] = '\0';
        }
        read++;
    }

    /*
    Stage 1 output
    */
    int dte[DATE_ITEM], tim[TIME_ITEM];
    double xy[POS_ITEM];
    char id[ID_LEN];
    readLocInfo(buffer[0], dte, tim, xy, id);
    printf("Stage 1\n");
    printf("==========\n");
    printf("Trajectory starting point: <%.6f, %.6f>\n", xy[0], xy[1]);
    printf("Trajectory starting time: %02d:%02d:%02d\n", tim[0], tim[1], tim[2]);
    printf("Trajectory starting date: %02d/%02d/%02d\n", dte[0], dte[1], dte[2]);
    printf("\n");

    /*
    Stage 2 output
    */
    // proxy for storing when reading line
    // does not matter what we store since only interested in lat and long
    int proxDate[DATE_ITEM], proxTime[TIME_ITEM];
    char proxID[ID_LEN];
    int i;
    // storing the two points (lat, long) in array
    double point1[2], point2[2], dist;
    printf("Stage 2\n");
    printf("==========\n");
    for (i = 1; i < read; i++){
        readLocInfo(buffer[i-1], proxDate, proxTime, point1, proxID);
        readLocInfo(buffer[i], proxDate, proxTime, point2, proxID);
        dist = distance(point1, point2);
        printf("Trajectory segment %02d: ", i);
        printf("length: %06.2f, ", dist);
        printf("speed: %05.2f\n", calculateSpeed(dist, INTERVAL));
    }
    printf("\n");

    /*
    Stage 3 output
    */
    printf("Stage 3\n");
    printf("==========\n");
    // Storage of IDs for segment that is the longest path
    char trackID[2][ID_LEN];
    // Temporary store IDs to ensure it method runs
    char tempID[2][ID_LEN];
    // Storing length that is longest
    double longest = 0;
    // Counting how many segments
    int number = 0;
    for (i = 1; i < read; i++){
        readLocInfo(buffer[i-1], proxDate, proxTime, point1, tempID[0]);
        readLocInfo(buffer[i], proxDate, proxTime, point2, tempID[1]);
        dist = distance(point1, point2);
        if (longest < dist){
            longest = dist;
            strcpy(trackID[0], tempID[0]);
            strcpy(trackID[1], tempID[1]);
        }
        number++;
    }
    printf("Number of trajectory segments: %d\n", number);
    printf("Trajectory segment with longest length: [%s, %s]\n",
    trackID[0], trackID[1]);

    return 0;
}
/**
* reads the line and save the information in the corresponding field array
* *line: the line to be read
* date[]: holds date information [dd, mm, yy]
* _time[]: holds time infomration [h, m, s]
* po[]: holds latitude and longitude [lat, long]
* id[]: holds id string
*/
void readLocInfo(char *line, int date[], int _time[], double pos[], char id[]){
    if (line && date && _time && pos && id){
        sscanf(line, "%d/%d/%d %d:%d:%d %lf %lf %s",
                &date[0], &date[1], &date[2],
                &_time[0], &_time[1], &_time[2],
                &pos[0], &pos[1], id);
    }
}
/**
* Calculate the distance of path between two pairs of lat and long
* pos1: lat and long of first point
* pos2: lat and long of second point
* return: the distance between the two positions
*/
double distance(double pos1[], double pos2[]){
    double chord_length = pow(sin(to_radian(pos2[0] - pos1[0])/2),2)
                        + cos(to_radian(pos1[0]))
                        * cos(to_radian(pos2[0]))
                        * pow(sin(to_radian(pos2[1] - pos1[1])/2),2);
    double angle_distance = 2*atan2(sqrt(chord_length), sqrt(1-chord_length));

    return EARTH_R*angle_distance;
}

/**
* Calculates the speed
* dist: the distance traveled
* seoconds: time in seconds to cover distance
*/
double calculateSpeed(double dist, int seconds){
    return dist/seconds;
}
/**
* Convert degrees to radian
* x: the degree to convert
* return: radian
*/
double to_radian(double x){
    return x*(PI_RAD/PI_DEG);
}
