/*
 ===================================================================================================================================================================================================
 Author Name:                Gary Mac Elhinney
 Student Number:             13465572
 Date finished:

 CODE BREAKDOWN:			(1)
 We're creating 64 linked lists, This array of 64 heads will allow us to access them when ever we want

 (2)
 This little loop will create a linked list for each character of the book. Once it's done that it'll add any further line a character appears on into it's respective
 linked list. Cool right?
 ====================================================================================================================================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
	int lineNum;
	struct node *next;
};

struct names {
	char fileNames[64][40];
};

struct names ObtainNames(FILE *fp, int numNames);
int ObtainNumNames(FILE *fp);
struct node* create_list(int lineNum, int i);
struct node* add_to_list(int lineNum);
void WriteToFile(int numNames, struct names variable);

struct node *head[64];											//----- 1 -----
struct node *curr = NULL;

int totalNames = 0;

int main() {
	FILE *fopen(), *filep1, *filep2;
	struct names variable;
	int numNames, i = 0, lineNumber;
	int bool;
	char temp[200];

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	filep1 = fopen("Les-Mis-Names.txt", "r");
	//filep1 = fopen("Les-Mis-Names-20.txt", "r");
	//filep2 = fopen("Les-Mis-4lines.txt", "r");
	filep2 = fopen("Les-Mis-Full-Text.txt", "r");

	numNames = ObtainNumNames(filep1);
	fseek(filep1, 0, SEEK_SET);
	variable = ObtainNames(filep1, numNames);

	while (i < numNames){										//----- 2 -----
		lineNumber = 1;
		bool = 1;
		while (fgets(temp, 200, filep2)) {
			if ((strstr(temp, variable.fileNames[i]) != 0) && (bool == 1)) {
				create_list(lineNumber, i);
				bool = 0;
			}

			else if ((strstr(temp, variable.fileNames[i]) != 0) && (bool == 0)) {
				add_to_list(lineNumber);
			}
			lineNumber++;
		}
		fseek(filep2, 0, SEEK_SET);
		i++;
	}

	fseek(filep2, 0, SEEK_SET);
	WriteToFile(numNames, variable);

	return 0;
}

int ObtainNumNames(FILE *fp) {
/*
 ======================================================================================================================================
 This function just counts all the names in the names file, it'll read a line, increment a counter and when there're no more lines
 it'll stop incrementing.
 ======================================================================================================================================
 */
	int totalNames = 0;
	char temp[50];

	while (fgets(temp, 50, fp) != 0) {
		totalNames++;
	}
	//printf("%d", totalNames);

	return totalNames;
}

struct names ObtainNames(FILE *fp, int numNames) {
/*
 ======================================================================================================================================
 All this little function does is grab a line from the names file, chomps the \n chracter and plop the name in an array in a struct
 Simples!
 ======================================================================================================================================
 */
	struct names variable;
	int counter = 0;

	while (fgets(variable.fileNames[counter], 40, fp) != 0) {
		if (variable.fileNames[counter][strlen(variable.fileNames[counter]) - 1] == '\n') {
			variable.fileNames[counter][strlen(variable.fileNames[counter]) - 1] = '\0';
			counter++;
			continue;
		}
		//printf("!%s!", variable.fileNames[counter]);
		counter++;
	}
	return variable;
}

struct node* create_list(int lineNum, int i) {
/*
 ======================================================================================================================================
 This function is called from main. It will create a list for each name and then set one of the head slots pointing to the start so
 we can access it later.

 It then points to the next node in the list, even though it's null. Yay! It's now linked.
 ======================================================================================================================================
 */
	struct node *ptr = (struct node*) malloc(sizeof(struct node));
	if (NULL == ptr) {
		printf("\n Node creation failed \n");
		return NULL;
	}

	ptr->lineNum = lineNum;
	ptr->next = NULL;
	curr = head[i] = ptr;

	return ptr;
}

struct node* add_to_list(int lineNum) {
/*
 ======================================================================================================================================
 This is called when a list has already been made for a name. It does the same thing but doesn't assign a head to it. It also links each
 node together.
 ======================================================================================================================================
 */
	struct node *ptr = (struct node*) malloc(sizeof(struct node));
	if (NULL == ptr) {
		printf("\n Node creation failed \n");
		return NULL;
	}
	ptr->lineNum = lineNum;

	ptr->next = NULL;

	curr->next = ptr;
	curr = ptr;

	return ptr;
}

void WriteToFile(int numNames, struct names variable) {
/*
 ======================================================================================================================================
 In this function we're going to compare all the name's nodes to every other name's nodes. However if we try to compare a node to itself
 we'll skip it.

 If the linenumber of each other node is the same as the current node, we'll print it to our Victory file!

 ======================================================================================================================================
 */
	FILE *fopen(), *fp;
	fp = fopen("Results.csv", "w");
	int i, j;
	i = j = 0;
	struct node* ptr1;
	struct node* ptr2;

	while (i < numNames) {//for every name
		ptr1 = head[i];	//set the pointer to be equal to the head of the linked list
		while (ptr1 != NULL) {//for every line number of the name
			j = 0;
			while (j < numNames) {//compare another linked list to i linked list 64 times
				if ((i == j) && (j != numNames - 1)) {//at last node 63 == 63 then j++ therefore 63==64 but 64 doesn't exist !!! CRASSHHHH
					j++;
				}
				ptr2 = head[j];

				while (ptr2 != NULL) {//check against every other occurrence of all other names
					if (ptr2->lineNum == ptr1->lineNum) {
						fprintf(fp, "%s, %s\n", variable.fileNames[i], variable.fileNames[j]);
					}
					ptr2 = ptr2->next;
				}
				j++;
			}
			ptr1 = ptr1->next;
		}
		i++;
	}
}
