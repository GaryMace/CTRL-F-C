/*
   ======================================================================================
   Student Name               : Gary Mac Elhinney
   Student Number 			  : 13465572
   Student Assignment 1 	  : Implementation of a Simple Slot machine

   HOW THE CODE WORKS         :  (1) . You'll notice i've included 4 header files, Each contains one or more commands needed to run the program.
   	   	   	   	   	   	   	   	       In particular, Time.h is needed for the rand(); function under ColumnResults();

   	   	   	   	   	   	   	   	 (2) . This contains all functions used in the program. Some as can be seen return values to main, others don't.
   	   	   	   	   	   	   	   	 	   Some don't need to return the value of scanf as it goes to a global variable and so there's no point.

   	   	   	   	   	   	   	   	 (3) . Listed at 3 is the struct used in the program. it contains the character array that "Pear", "Apple" and
   	   	   	   	   	   	   	   	       "Orange" are contained in.

   	   	   	   	   	   	   	   	 (4) . This is the list of global variables, and whilst I understand that they shouldn't really be used they make
   	   	   	   	   	   	   	   	 	   referring to a common variable much simpler. User_bet for instance is used in nearly every function!

   	   	   	   	   	   	   	   	 (5) . I spent quite some time trying to figure out a way to loop all the functions. I didn't think it would be
   	   	   	   	   	   	   	   	 	   as simple as putting them in a loop! Which is kind of ironic really. So this will literally just re-loop
   	   	   	   	   	   	   	   	 	   all the functions until loop becomes false.

   	   	   	   	   	   	   	   	 	   i.e. It never does become false, i originally had to it do just that, but it was much easier just to include
   	   	   	   	   	   	   	   	 	   exit(0) functions!


   =====================================================================================

 */

#include <stdio.h>								//======= 1 ========
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Functions
char ColumnResults ();							//======= 2 =======
void ContinuePlaying();
void BettingSystem();
void Welcome();
void InputTester ();
int AskToPlayAgain();

//Structures
typedef struct {		 						//======= 3 =======
	char fruits[5][10];		//can hold 5 words, 10 characters long
}Column;

typedef struct {
	char dummy[10];
}Dummy;

//Global variables								//======= 4 =======
float user_bet;
float credits;
int counter0;
int counter1;
int counter2;
int loop;
int boolean;

int main() {
	loop = 1;
	credits = 10;
	boolean = 0;
	setbuf(stdin, NULL);				// This disables IO buffering within Eclipse which is needed since it is incapable of doing it correctly
	setbuf(stdout, NULL);

	printf("********Welcome to my Slot Machine********\n");
	Welcome();							// Runs the Welcome function

	while( loop == 1) {							//======= 5 =======
		InputTester();
		ColumnResults();
		BettingSystem();

		AskToPlayAgain();
		ContinuePlaying();
	}
	return 0;
}


void Welcome() {
/*
	 =================================================================================
	 This function doesn't return any values, it doens't need to! since user_bet is global
	 it saves needless extra lines in other functions and in main.

	 It basically just welcomes the user, displays default credits and asks for a bet.

	 ==================================================================================
 */
	printf("\nYou have %.1f credits remaining ", credits);
	printf("\nHow many credits do you wish to bet?\t\n");
	scanf("%f", &user_bet);
}

void InputTester () {
/*
 	======================================================================================
 	This was one of the harder functions to define. It will take the user's bet and check to
 	see if it is in the range 2 - AVAILABLE CREDITS, if it isn't it ask's for another bet!

 	It also checks to see if the bet was indeed an integer or a char. The problem is this
 	particular error handling code only works for the very first input. I couldn't for the
 	life of me figure a way around it, I also asked the TA's but they also couldn't see a
 	way to do it.

 	If the input is in the correct range the loop terminates
 	======================================================================================
 */
	Dummy store;
	int i = 0;
	while (boolean == 0) {
		gets(store.dummy);

		if (user_bet > 2 && user_bet <= credits) {
			boolean = 1;
		}

		else if( user_bet < 2 || user_bet > credits ) {
			printf("Invalid bet, Entry must be greater than 2 and <= current credits. Try again!\t");
			scanf("%f", &user_bet);
		}

		else if((store.dummy[i] >= 'a' && store.dummy[i] <= 'z') || (store.dummy[i] >= 'A' && store.dummy[i] <= 'Z')) {
				printf("Invalid bet, Entry must be greater than 2 and <= current credits. Try again!\t");
				scanf("%f", &user_bet);
		}
		   i++;
	}
}

char ColumnResults() {
/*
 	==================================================================================================
 	The strcpy command copies the Strings "Pear", "Apple" and "Orange" into the first slots of
 	the struct array fruits..

 	Loop gives "j" a random number from 0 - 2, Whatever j is determines which
 	counter increments, If counter 0 is three then obviously 3 "Pears" have appeared!

 	Again Counter 0,1,2 are global as they're referred to elsewhere, unlike i and j
 	==================================================================================================
 */
	Column variable;
	int i;
	int j;
	counter0 = 0;
	counter1 = 0;
	counter2 = 0;

	srand(time(NULL));

	strcpy(variable.fruits[0], "Pear");
	strcpy(variable.fruits[1], "Apple");
	strcpy(variable.fruits[2], "Orange");

	for(i = 0; i < 3; i++) {
		j = rand()%3;
		if( j == 0) {
			counter0++;
		}
		if( j == 1) {
			counter1++;
		}
		if (j == 2) {
			counter2++;
		}
		printf("|%s| ", variable.fruits[j]);			//Print out the random slot j
	}
	return 0;
}

void BettingSystem() {
/*
	 =========================================================================================
	 So this function determines the winnings based on the counters used above in ColumnResults();

	 the first if gives 2x credits entered if any of the counters are equal to 3
	 The second gives half credits entered if any of the counters are equal to 2
	 The final gives no credits to the user, but takes away the amount entered if no faces matched
	 ========================================================================================
*/
	float winnings;

	if(counter0 == 3|| counter1 == 3 ||counter2 == 3) {
		winnings = user_bet*2;
		credits = credits + winnings;
		printf("\nWell Done!, You've WON %.0f credits", winnings);
	}

	else if(counter0 == 2 || counter1 == 2 || counter2 == 2) {
		winnings = user_bet/2;
		credits = credits + winnings;
		printf("\nWell Done!, You've WON %.1f credits", winnings);
	}

	else {
		credits = credits - user_bet;
		printf("\nUnfortunately, you've LOST %.0f credits ", user_bet);
	}
}

int AskToPlayAgain() {
/*
 	============================================================================================
 	So this program asks the user if they want to play again, the dummy variable discards the \n
 	character.

 	If the input is y and the user has more than 2 credits it will return loop's value to main and
 	end the function there!

 	Therefore, this function does need to return a value, Sweet!!
 	If the user enters "y" but has under 2 credits the program closes

 	If "n" is entered the function determines if the user made a loss or a winning overall
 	===========================================================================================
 */
	char dummy;
	char answer;
	float final_credits;
	printf("\n\nDo you wish to continue playing the Slot Machine (y/n)?\t");
	scanf(" %c%c", &answer, &dummy);

	if( (answer== 'y' || answer == 'Y') && credits > 2.0) {
		loop = 1;
	}

	else if( (answer == 'y' || answer == 'Y') && credits <= 2.0) {
		printf("\n\n*****Sorry, you haven't enough credits to continue playing. Bye!!*****");
		exit(0);
	}

	else if((answer == 'n' || answer == 'N') && (credits- 10 < 0)) {
		final_credits = credits - 10;
		printf("\n\n*****End of game: You made a LOSS of %.1f credits!*****", final_credits );
		exit(0);
	}

	else if( ( answer == 'n' || answer == 'N') && (credits- 10 > 0) ) {
		final_credits = credits - 10;
		printf("\n\n*****End of game: You WON %.1f credits!*****", final_credits);
		exit(0);
	}

	else {
		printf("\n******You didn't enter Y or N, program will now shut down******");
		exit(0);
	}

	return loop;											// loop  = 1 is returned if it's if statement is satisfied otherwise it exits!
}

void ContinuePlaying() {
/*
 	===============================================================================================
 	This function asks the user for there next bet after saying they want to continue. If, however,
 	they enter a character the program closes. because user_bet will remain 0 unless a new integer
 	is entered.

 	I tried to make it so it would loop until an integer was entered but the loop kept skipping over the scanf!
 	===============================================================================================
*/
	printf("\nYou have %.1f credits remaining ", credits);
	printf("\nHow many credits do you wish to bet?\t\n");
	user_bet = 0;
	scanf("%f", &user_bet);
	if(user_bet < 1) {
		printf("\n******Invalid bet, you didn't enter a character, Program will now shut down!******");
		exit(0);
	}
}
