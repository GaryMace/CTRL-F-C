/*
 ============================================================================
 Name        : GPS analysis.c
 Author      : Gary Mac Elhinney
 ============================================================================
 */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define d2r (3.14159 / 180.0)

// The node structure for storing paths
struct node {
	double start;
	double lat;
	double lon;
	double time;
	double elevation;
	char timeString[22];
	struct node *next;
};

// The structure for storing times containing hr, min and sec fields
struct timeStr {
	int hr;
	int min;
	int sec;
};

double haversine_m(double lat1, double long1, double lat2, double long2);
int openFileAndLoadData();
char *readStringAfterToken(char *txtstr, char *tkn, char *res, int len,int steps);
double readDoubleAfterToken(char *txtstr, char *tkn, int steps);
double calculate_tot_dist(struct node *lh);
struct node* create_list(double lat, double lon, double elevation, char *timeStr);
struct node* add_to_list(double lat, double lon, double elevation, char *timeStr);
struct timeStr *timeStrFromString(char *tstring, struct timeStr *tstruct);
struct node* PrintSplits(double distance, double elevation, double timeValue, int splitNum);
int timeDiffV2(struct timeStr *t1, struct timeStr *t2);

struct node *head = NULL;
struct node *curr = NULL;
double firstElevation = 0;
double fastestPace = 100.0;	//Just so it's guaranteed to be changed to a smaller value we'll set the value to be ridiculous
double slowestPace = 0.0;

char startTimeStr[25];	//Hold the start and end time strings as global vars.
char finishTimeStr[25];

//Working files
//const char *GPX_FILE_PATH = "./inputFiles/Zell75k.gpx";
//const char *GPX_FILE_PATH = "./inputFiles/Howth-Cross.gpx";
const char *GPX_FILE_PATH = "./inputFiles/Run4.9k.gpx";

//These files are purposely supposed to not work
//const char *GPX_FILE_PATH = "./inputFiles/Test1.gpx";
//const char *GPX_FILE_PATH = "./inputFiles/Test2.gpx";
//const char *GPX_FILE_PATH = "./inputFiles/Test3.gpx";


int main(void) {
	openFileAndLoadData();

	calculate_tot_dist(head);
	return EXIT_SUCCESS;
}

// Calculate distance between two points expressed as lat and long using
// Haversine formula.
double haversine_m(double lat1, double long1, double lat2, double long2) {
	double dlong = (long2 - long1) * d2r;
	double dlat = (lat2 - lat1) * d2r;
	double a = pow(sin(dlat / 2.0), 2) + cos(lat1 * d2r) * cos(lat2 * d2r) * pow(sin(dlong / 2.0), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double d = 6367137 * c;
	return d;
}

//Function that is called once at the start to read in the character names.
int openFileAndLoadData() {
	int lineNum = 0;
	char theLine[120];
	char endOfHdr[10] = "<trkseg>";
	char endOfData[10] = "</trkseg>";
	char *timeStrPtr;
	char tempTimeStr[30];

	FILE *fpn = fopen(GPX_FILE_PATH, "r+"); /* open for reading */
	if (fpn == NULL) { /* check does file exist etc */
		printf("Cannot open %s for reading. \n", GPX_FILE_PATH);
		printf("Set up GPX file at %s and restart. \n", GPX_FILE_PATH);
		perror("Error opening GPX file. \n");
	}

	else {
		// First work through the header in the GPX file.
		// Assume header finishes at "</trkseg>" stored in endOfHdr.

		while (fgets(theLine, sizeof(theLine), fpn) && ((strncmp(theLine, endOfHdr, 7) != 0))) {
			//Skip through the file until "<trkseg>" is reached (endOfHdr).
			lineNum++;
		}

		// Read the text data and store the lat, lon and time data in a linked list.
		if (fgets(theLine, sizeof(theLine), fpn) && ((strncmp(theLine, endOfData, 7) == 0))) {
			printf("Error!! GPX file contains no data");
			exit(0);
		}
		while (fgets(theLine, sizeof(theLine), fpn) && ((strncmp(theLine, endOfData, 7) != 0))) //finish when "</trkseg>" reached (endOfData)
		{
			timeStrPtr = readStringAfterToken(theLine, "<time>", tempTimeStr, 20, 6);	//gets time data from file first 20 characters skipping the <time> token
			add_to_list(readDoubleAfterToken(theLine, "lat=", 5), readDoubleAfterToken(theLine, "lon=", 5),readDoubleAfterToken(theLine, "<ele>", 5), timeStrPtr);

			lineNum++;
		}
	}
	fclose(fpn);
	return lineNum;
}

// a function that will calculate the total length of the track.

double calculate_tot_dist(struct node *lh)// ptr = ptr->elevation will cycle through each struct node and add all the different elevations
{
	int finalTime = 0;
	int splitNum = 1;
	double lat1 = 0, lon1 = 0;
	double elev = 0.0;
	double timeValue = 0;
	double pathLen = 0;
	double firstElevation = 0.0;
	double finalElevation;
	double finalPathLen;
	double tempElev = 0;
	double finalElevBeforeSplit = 0.0;
	double elevPass = 0.0;
	double averagePace = 0;
	struct node *ptr = lh;
	struct timeStr tm1, tm2, timestring1, timestring2;
	char currTime[25];
	char PreviousTime[25];

	strcpy(startTimeStr, lh->timeString);
	strcpy(PreviousTime, startTimeStr);
	while (ptr != NULL) {

		if (lat1 == 0) {
			// First node

			lat1 = ptr->lat;
			lon1 = ptr->lon;
			elev = ptr->elevation;//Both elev and temp are 0.0000... for the first elevation here? why
			tempElev = ptr->elevation;
			firstElevation = ptr->elevation;
			ptr = ptr->next;
		}

		else {
			pathLen += haversine_m(lat1, lon1, ptr->lat, ptr->lon);
			finalPathLen += haversine_m(lat1, lon1, ptr->lat, ptr->lon);
			lat1 = ptr->lat;
			lon1 = ptr->lon;
			elev = ptr->elevation;

			strcpy(currTime, ptr->timeString);							//  ^
			timestring1 = *timeStrFromString(PreviousTime, &timestring1);//	|		This series of commands get's the time difference between..
			timestring2 = *timeStrFromString(currTime, &timestring2);//	|		the current time read and the time from the previous linked list..
			strcpy(PreviousTime, currTime);	//	|		this will therefore be the time interval taken between the distances.
			timeValue += timeDiffV2(&timestring1, &timestring2);		//	^

			//THis does the same as above, gets the difference in elevation between two consecutive linked lists
			//This is for the final elevation, above is for the function PrintSplits
			finalElevBeforeSplit = ptr->elevation;

			if (pathLen >= 1000 || ptr->next == NULL) {//if distance exceeds 1000 or there isn't another linked list left, i.e. the last list has been used
				elevPass = finalElevBeforeSplit - tempElev;
				PrintSplits(pathLen, elevPass, timeValue, splitNum);
				tempElev = finalElevBeforeSplit;
				splitNum++;
				timeValue = 0;			//reset the parameters for next split
				elevPass = 0;
				pathLen = 0;
			}
			else if (pathLen == 0) {
				elevPass = 0;
				pathLen = 0;
				PrintSplits(pathLen, elevPass, timeValue, splitNum);
			}

			finalElevation = ptr->elevation - firstElevation;
			ptr = ptr->next;
		}
	}
	printf("\t\t----------------------------------------------------\n\n\n");//From here on prints the overall stats
	printf("\t\t\t   ----------End Of Splits----------\n\n");

	printf("\t\t\t   ---------------------------------\n");
	printf("\t\t\t   |\t      Overall Stats\t   |\n");
	printf("\t\t\t   ---------------------------------\n");
	strcpy(finishTimeStr, curr->timeString);

	tm1 = *timeStrFromString(startTimeStr, &tm1);			//Overall time taken
	tm2 = *timeStrFromString(finishTimeStr, &tm2);
	finalTime = timeDiffV2(&tm1, &tm2);

	printf("\t\t\t   | Elapsed Time:    %d sec\t   |\n", finalTime);
	printf("\t\t\t   | Path Length:    %5.0f m\t   |\n", finalPathLen);

	averagePace = timeDiffV2(&tm1, &tm2) / finalPathLen * 1000.0 / 60.0;//Get average pace
	if (finalPathLen >= 1) {
		printf("\t\t\t   | Average Pace:    %4.2lf m/km    |\n", averagePace);
		printf("\t\t\t   | Overall Elevation: %4d m     |\n", (int) finalElevation);
		printf("\t\t\t   | Fastest Pace:    %4.2lf m/km    |\n", fastestPace);
		printf("\t\t\t   | Slowest Pace:    %4.2lf m/km    |\n", slowestPace);
		printf("\t\t\t   ---------------------------------\n\n");
	}

	else {
		printf("\t\t\t   ---------------------------------\n\n");
	}

	return pathLen;
}

// This function will return a  substring from string txtstr.
// The string will be searched for the first occurrence of tkn
// and then a substring on length len will be returned starting steps spaces from the
// start of tkn.
// If the token is not found in the string it returns a null pointer.

char *readStringAfterToken(char *txtstr, char *tkn, char *res, int len, int steps) {
	//target string, token, length of substring and steps beyond start of token
	char *tmpstr;
	char *ret = NULL;

	tmpstr = strstr(txtstr, tkn);		//Search read in line for <time>

	if (tmpstr) // Checking to make sure the pointer is not NULL, i.e. strstr returned something.
		ret = strncpy(res, (tmpstr + steps), len);//if it finds <time> it copys all the data after the header into the variable ret and returns it

	return ret;
}

// This function will return a double from string txtstr. The string will be searched
// for the first occurrence of tkn and the reading of the double will start steps
// places after the start of tkn.
// If the token is not found in the string it returns a value of -1.
// If there is no number after the token it returns 0.

double readDoubleAfterToken(char *txtstr, char *tkn, int steps) { //target string, token and steps beyond start of token
	char *tmpstr;
	char *rem;
	double res = -1;

	tmpstr = strstr(txtstr, tkn);//Searches line of file for occurance of tkn... lat=" "...

	if (tmpstr) // Checking to make sure the pointer is not NULL, i.e. strstr returned something.
		res = strtod((tmpstr + steps), &rem);

	return res;
}

// Create the list to be used to store the data
struct node* create_list(double lat, double lon, double elevation, char *timeStr) {
	char ts[35];
	strcpy(ts, timeStr);
	struct node *ptr = (struct node*) malloc(sizeof(struct node));

	if (NULL == ptr) {
		printf("\n Node creation failed \n");
		return NULL;
	}

	ptr->lat = lat;
	ptr->lon = lon;
	ptr->elevation = elevation;
	strcpy(ptr->timeString, timeStr);

	ptr->next = NULL;

	head = curr = ptr;
	return ptr;
}

// Add nodes to the main data list
struct node* add_to_list(double lat, double lon, double elevation, char *timeStr) {
	char ts[35];
	strcpy(ts, timeStr);
	//printf("\n adding node to list with time as %s\n", timeStr);

	if (NULL == head) {
		return (create_list(lat, lon, elevation, timeStr));
	}

	struct node *ptr = (struct node*) malloc(sizeof(struct node));
	if (NULL == ptr) {
		printf("\n Node creation failed \n");
		return NULL;
	}
	ptr->lat = lat;
	ptr->lon = lon;
	ptr->elevation = elevation;
	strcpy(ptr->timeString, timeStr);

	ptr->next = NULL;

	curr->next = ptr;
	curr = ptr;

	return ptr;
}

// A function to populate a timeStr time structure from an GPX time string.
// e.g. "2013-09-12T15:59:18Z"
struct timeStr *timeStrFromString(char *tstring, struct timeStr *tstruct) {

	sscanf(tstring + 11, "%d", &tstruct->hr);
	sscanf(tstring + 14, "%d", &tstruct->min);
	sscanf(tstring + 17, "%d", &tstruct->sec);

	return tstruct;
}

struct node* PrintSplits(double distance, double elevation, double timeValue,
		int splitNum) {
	struct node *ptr = NULL;
	if (timeValue == 0) {
		printf("\n\n\t\t          ----------Time Splits----------\n\n");
		printf("\t\t----------------------------------------------------\n");
		printf("\t\t| Split num | Pace(m/Km)| Speed(km/h)| Elevation(m)|\n");
		printf("\t\t----------------------------------------------------\n");
		printf("\t\t|\t  %2.d|\t     0:0|         0.0|\t    %7d|\n", splitNum,
				(int) elevation);
		return ptr;
	}

	static int splitHeader = 0;
	double PaceInDecimal = ((timeValue / distance) * 1000.0) / 60.0;//Puts pace in decimal form
	double tempPace2 = floor(PaceInDecimal);//rounds it down to nearest number
	double PaceInTimeInterval = ((PaceInDecimal - tempPace2) * 60);	//Changes the decimal point to ratio between 1 - 60
	double speed = (distance / timeValue) * 60 * 60 / 1000;

	if (PaceInDecimal < fastestPace) { //GLOBAL VARIABLE: Gets users fastest pace
		fastestPace = PaceInDecimal;
	}

	if (PaceInDecimal > slowestPace) {//GLOBAL VARIABLE: Gets users slowest pace
		slowestPace = PaceInDecimal;
	}

	if (splitHeader == 0) {//As a result of this being a static int, this will only print once
		firstElevation = elevation;

		printf("\n\n\t\t          ----------Time Splits----------\n\n");
		printf("\t\t----------------------------------------------------\n");
		printf("\t\t| Split num | Pace(m/Km)| Speed(km/h)| Elevation(m)|\n");
		printf("\t\t----------------------------------------------------\n");
	}

	if (elevation - floor(elevation) > 0.5)	 {//This will round the elevation up or down depending on it's decimal being greater than or less than 0.5
		elevation = ceil(elevation);
	}

	else if (elevation - floor(elevation) < 0.5) {
		elevation = floor(elevation);
	}

	printf("\t\t|\t  %2.d|\t  %3d:%02d|       %5.2lf|\t    %7d|\n", splitNum, (int) tempPace2, (int) PaceInTimeInterval, speed, (int) elevation);
	splitHeader = 1;//This stops the header from being printed each time, as it's static
	return ptr;
}

// A function to return the time difference in seconds between
// two timeStr time structures.
// It assumes the two times are from the same day.

int timeDiffV2(struct timeStr *t1, struct timeStr *t2) {
	int ret = 0, s1 = 0, s2 = 0;

	s1 = t1->hr * 3600 + t1->min * 60 + t1->sec;
	s2 = t2->hr * 3600 + t2->min * 60 + t2->sec;
	ret = s2 - s1;
	return ret;
}
