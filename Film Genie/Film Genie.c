/*
 ===================================================================================================================================================================================================
 Author Name:                Gary Mac Elhinney
 Student Number:             13465572
 Date finished:              3rd March 2014 ( My birthday :D yay! )

 CODE BREAKDOWN:			(1)
 In this structure i've made 4 2-d arrays, so what's happening here? In the beginning all the words from the file will be stored in the words[][] array
 the second one, encryption, will be assigned all words from the first but will then encrypt them later.

 The last two, and yes I realise i spelt encryption wrong for these .... , will store JUST the selected random movie and the corresponding encrypted movie
 Num of films is just to know how many films there're

 (2)
 I'm defining structure functions here, remember a structure variable can act as a sort of global variable, here i'll be able to access the structure from
 anywhere in my program, these are really useful!
 I'm also passing the previous structure variable used as a parameter for the new function so I can keep accessing the structure

 (3)
 I only need to recur two of my functions, the films and the corresponding encrypted film will be saved and so running them again is pointless. All i need to
 use again is the GetRandomFilm function and then check if the users guesses are valid

 (4)
 This function simply opens the file, if at all possible, and then displays a message to confirm it's been opened.

 (5)
 This function, Encryption, will assign all the words from the first array (words) to the second function, (encryption) effectively making a copy of it at first,
 it will then encrypt every word in the array, Later on we'll see why i do this

 (6)
 The GetRandomMovie function will get a random number, courtesy of rand() and will assign unincrypted_selected_movie equal to it. This is one of the two recurring
 functions.

 (7)
 I do realise how long this function is, over 200 ish lines?, the reason being it was easier to test the users input for all the cases that this assignment asked
 for all in the same function rather than passing parts of it across, the vast majority of the code is just for testing the users input! so it was logical to have
 it all in the same function.
 ====================================================================================================================================================================================================
 */

#include <stdio.h>
#include <strings.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

//define max size each array can be
#define MAX_WORDS 150
#define MAX_LENGTH 50

//structures
struct WordStructure {											// ===== 1 =====
	char words[MAX_WORDS][MAX_LENGTH];
	char encryption[MAX_WORDS][MAX_LENGTH];
	int num_of_films;
	char unincrypted_selected_movie[1][35];
	char incrypted_selected_movie[1][35];
};

//functions
struct WordStructure ObtainWords(FILE *fp);
struct WordStructure Encryption(struct WordStructure variable); // ===== 2 =====
struct WordStructure GetRandomMovie(struct WordStructure variable2);
struct WordStructure UserGuessesMovie(struct WordStructure variable3);

int main() {
	/*
	 ======================================================================================================
	 Main doesn't really do much other than welcoming the user, getting the file they want to open and
	 calling the functions needed.
	 ======================================================================================================
	 */
	char file_title[50];
	int loop = 1;
	FILE *fopen(), *fp;

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	printf("***********************************\n");
	printf("**      Welcome to FilmGenie     **\n");
	printf("***********************************\n\n");

	printf("Enter a file to open!:\t");
	gets(file_title);

	fp = fopen(file_title, "r");
	if (fp == NULL) {
		printf("Cannot open %s for reading\n", file_title);
		exit(0);
	}

	struct WordStructure variable = ObtainWords(fp);//declares a structure variable equal to structure variable used in function
	struct WordStructure variable2 = Encryption(variable);

	while (loop == 1)											// ===== 3 =====
	{
		struct WordStructure variable3 = GetRandomMovie(variable2);
		UserGuessesMovie(variable3);
	}

	fclose(fp);
	return 0;
}

struct WordStructure ObtainWords(FILE *fp) {					// ===== 4 =====
	/*
	 ==================================================================================================================
	 This function will test if the users inputed file name actually exists, if it does it is opened. If the file exits
	 but it is empty then the programme closes. It will then put all the movies in the file into separate slots in a
	 2-d array.
	 ===================================================================================================================
	 */
	struct WordStructure variable;
	char temp;
	int i = 0, j = 0;

	temp = getc(fp);
	if (temp == EOF) {
		printf("That file is empty, Goodbye");
		exit(0);
	}

	else {
		printf("****File opened Succesfully!****\n\n");
	}

	while (temp != EOF) {
		if (temp == '\n') {
			variable.words[j][i] = '\0';
			temp = getc(fp);
			i = 0;
			j++;
		}
		variable.words[j][i] = temp;
		i++;
		temp = getc(fp);

	}
	variable.words[j][i] = '\0';
	i = 0;
	variable.num_of_films = j;

	return variable;
}

struct WordStructure Encryption(struct WordStructure variable) {  // ===== 5 =====
	/*
	 ===================================================================================================================
	 In this function the number of films in the file is obtained and then all the words in the encryption 2-d array are
	 encrypted with # symbols
	 ===================================================================================================================
	 */
	int i = 0, j = 0;

	for (j = 0; j < variable.num_of_films; j++) {
		for (i = 0; i < MAX_LENGTH; i++) {
			variable.encryption[j][i] = variable.words[j][i];
		}

	}

	i = 0;
	for (j = 0; j < variable.num_of_films; j++) {
		for (i = 0; i < MAX_LENGTH; i++) {
			if (variable.encryption[j][i] == '\0') {
				variable.encryption[j][i] = '\0';
				break;
			}

			else if (variable.encryption[j][i] >= 'a'
					&& variable.encryption[j][i] <= 'z') {
				variable.encryption[j][i] = '#';
			}

			else if (variable.encryption[j][i] >= 'A'
					&& variable.encryption[j][i] <= 'Z') {
				variable.encryption[j][i] = '#';
			}

			else {
				//Do nothing to that character
			}
		}
	}
	return variable;
}

struct WordStructure GetRandomMovie(struct WordStructure variable2)	{// ===== 6 =====
	/*
	 ============================================================================================================
	 In this function j is used as a random number, the array that contains only one film is then assigned the
	 film located at the randomly chosen "j" slot. the corresponding encrypted film is also put into the other
	 holding array.
	 ============================================================================================================
	 */
	srand(time(NULL));
	int j, i;

	j = rand() % variable2.num_of_films;

	//Gets the unincrypted word form file
	for (i = 0; i < MAX_LENGTH; i++) {
		variable2.unincrypted_selected_movie[0][i] = variable2.words[j][i];
	}
	variable2.unincrypted_selected_movie[j][i] = '\0';

	//Gets the incrypted word from file
	for (i = 0; i < MAX_LENGTH; i++) {
		variable2.incrypted_selected_movie[0][i] = variable2.encryption[j][i];
	}
	variable2.incrypted_selected_movie[j][i] = '\0';

	return variable2;
}

struct WordStructure UserGuessesMovie(struct WordStructure variable3) { // ===== 7 =====
	/*
	 ==============================================================================================================================
	 As can be seen this is a very long function but it made it easier to have it all in one than in separate as there would be a lot
	 more variables etc.

	 - The first thing that happens is the random film is printed in encrypted form, the user is prompted to enter either "c" for a
	 character bet or "f" for a film bet.

	 - If "c" is entered they are prompted for a character, if the character is actually a string, and error message is displayed
	 if it's a number the same, etc. If the character guess isn't contained in the string a message is displayed, if the character
	 IS in the string then it reprints the movie title with the new characters in place.

	 - If the user enters "f" they are prompted to enter the full movie title, if they get it wrong a message is displayed, if they enter
	 a number an error message is displayed. They have 5 attempts at getting this right, if they get it right on their first, second...
	 ..... fifth attempt a switch statement is used to print out a message.

	 - When the user eventually gets the entire film right through using characters or the entire film a message is displayed asking
	 them if they want to play again, if they do a new film is chosen and the process repeats
	 ==============================================================================================================================
	 */

	char yes_or_no;
	char choice[10];
	char dummy;
	char input[10];
	char string_temp[35];
	char film_title[1][35];

	int i;
	int counter = 0;
	int word_size = 0;
	int playagain = 1;
	int truth_value;
	int guess = 0;
	int guess_length = 0;
	int guess_left = 5;

	printf("Your movie is:\t %s\n\n", variable3.incrypted_selected_movie[0]);

	while (playagain == 1) {

		//Check if input is valid...i.e. a "c" or an "f"
		printf(
				"Enter either 'c' to guess a character or 'f' to guess the entire film!:\t");
		gets(choice);

		guess_length = 0;
		//Checks if input is c or f and not any longer than that
		for (i = 0; choice[i] != '\0'; i++) {
			guess_length++;
		}
		if (guess_length > 1) {
			continue;
		}

		//****** For Character guess! ******
		if (choice[0] == 'c' || choice[0] == 'C') {
			counter = 0;
			word_size = 0;
			printf("Enter a character guess!:\t");
			gets(input);

			//Is the user input a string or a single character?
			for (i = 0; input[i] != '\0'; i++) {
				counter++;
			}

			if (counter > 1) {
				printf(
						"\n\n****You entered a string, I was expecting either \"c\" or \"f\"****\n\n");
				continue;
			}
			counter = 0;

			// -->   Change both user input and generated film title to lower case
			for (i = 0; film_title[0][i] != '\0'; i++) {
				film_title[0][i] = tolower(film_title[0][i]);
			}

			for (i = 0; variable3.unincrypted_selected_movie[0][i] != '\0';
					i++) {
				variable3.unincrypted_selected_movie[0][i] = tolower(
						variable3.unincrypted_selected_movie[0][i]);
			}
			//<--

			//Gets Word Size!
			for (i = 0; variable3.unincrypted_selected_movie[0][i] != '\0';
					i++) {
				word_size++;
			}

			//Checks if user input is contained in string
			for (i = 0; i < word_size; i++) {
				if (input[0] == variable3.unincrypted_selected_movie[0][i]) {
					variable3.incrypted_selected_movie[0][i] = input[0];
				}

				else {
					counter++;
				}
			}

			//Does the user entered char exist in the string?
			if (counter == word_size) {
				printf("\n\nYour movie is:\t %s",
						variable3.incrypted_selected_movie[0]);
				printf(
						"\n**That letter is not contained in the string! Try again**\n\n");
				continue;
			}
			counter = 0;
			truth_value = strcmp(variable3.incrypted_selected_movie[0],
					variable3.unincrypted_selected_movie[0]);

			//Compare strings to see if they won
			if (truth_value == 0) {
				printf("%s\n", variable3.incrypted_selected_movie[0]);
				printf("******Well Done, you Won!******");

				while ((yes_or_no != 'n' || yes_or_no != 'N')
						|| (yes_or_no != 'y' || yes_or_no != 'Y')) {
					printf("\n\nDo you want to keep playing?:\t");
					scanf("%c%c", &yes_or_no, &dummy);

					if (yes_or_no == 'n' || yes_or_no == 'N') {
						exit(0);
					}

					else if (yes_or_no == 'y' || yes_or_no == 'y') {
						playagain = 0;
						continue;
					}

					else {
						printf("Error, you didn't enter \"y\" or \"n\"\n");
					}
				}
			}
			printf("Your movie is:\t %s\n\n",
					variable3.incrypted_selected_movie[0]);

			while ((yes_or_no != 'n' || yes_or_no != 'N')
					|| (yes_or_no != 'y' || yes_or_no != 'Y')) {
				printf("\nDo you want to keep guessing?:\t");
				scanf(" %c%c", &yes_or_no, &dummy);

				if (yes_or_no == 'n' || yes_or_no == 'N') {
					exit(0);
				}

				else if (yes_or_no == 'y' || yes_or_no == 'y') {
					break;
				}

				else {
					printf("Error, you didn't enter \"y\" or \"n\" ");
				}

			}
		}

		// ******For a File guess******
		else if (choice[0] == 'f' || choice[0] == 'F') {
			counter = 0;
			word_size = 0;
			//remove \n from previous input and get new input

			printf("Enter your Film title guess:\t");
			gets(string_temp);

			//Gets Word from files size!
			for (i = 0; variable3.unincrypted_selected_movie[0][i] != '\0';
					i++) {
				word_size++;
			}

			//put input into testing array
			string_temp[word_size] = '\0';
			for (i = 0; i < word_size; i++) {
				film_title[0][i] = string_temp[i];
			}
			film_title[0][i] = '\0';

			//change all characters in user guess & randomly obtained film to lower case
			for (i = 0; film_title[0][i] != '\0'; i++) {
				film_title[0][i] = tolower(film_title[0][i]);

			}

			for (i = 0; variable3.unincrypted_selected_movie[0][i] != '\0';
					i++) {
				variable3.unincrypted_selected_movie[0][i] = tolower(
						variable3.unincrypted_selected_movie[0][i]);
			}

			//Is the inputed word the same as the stored word
			for (i = 0; i < word_size; i++) {
				if (input[i] == variable3.unincrypted_selected_movie[0][i]) {
					counter++;
				}

			}
			truth_value = strcmp(film_title[0],
					variable3.unincrypted_selected_movie[0]);

			//Compare strings to see if they won
			if (truth_value == 0) {
				switch (guess_left) {
				case 5:
					printf(
							"\n\n*****Well Done you got it right first time, but sorry we're out of medals..*****");
					break;
				case 4:
					printf(
							"\n\n*****You got it on your second guess, I could've done better.. *****");
					break;
				case 3:
					printf(
							"\n\n*****Third Guess?? You might as well have not played at all..*****");
					break;
				case 2:
					printf(
							"\n\n*****Fourth Guess. I'm counting that one as a loss, #sorry #notreally*****");
					break;
				case 1:
					printf(
							"\n\n*****FIFTH GUESS? This is actually embarrassing, just quit now*****");
					break;
				default:
					printf(
							"\n\n*****Game over, No comment... oh wait! You're a failure*****");
					break;
				}
				printf("\nDo you want to keep playing ?:\t");
				scanf("%s%c", &yes_or_no, &dummy);

				if (yes_or_no == 'n' || yes_or_no == 'N') {
					printf(
							"\n\n****Thanks for playing! hope you enjoyed it !****\n");
					exit(0);
				}

				else if (yes_or_no == 'y' || yes_or_no == 'y') {
					playagain = 0;
					continue;
				}

				else {
					printf("Error, you didn't enter \"c\" or \"f\" ");

				}

			}

			//If strings not equal then decrement file word guesses
			else if (counter != word_size) {
				printf("\n\n****************************************\n");
				printf("*    Unfortunately that's incorrect    * ");
				guess_left--;
				printf("\n* You have %d Film title guesses left ! *\n",
						guess_left);
				printf("****************************************");
				if (guess_left == 0) {
					printf("\nYour movie was:\t **%s**\n\n",
							variable3.unincrypted_selected_movie[0]);
					printf(
							"\n\n****You have no guesses left ;( program will close!****");
					exit(0);
				}
				guess++;
			}

			printf("\nDo you want to keep guessing?:\t");
			scanf("%s%c", &yes_or_no, &dummy);
			fflush(stdin);

			if (yes_or_no == 'n' || yes_or_no == 'N') {
				printf(
						"\n\n****Thanks for playing! hope you enjoyed it !****\n");
				exit(0);
			}

			else if (yes_or_no == 'y' || yes_or_no == 'y') {
				counter = 0;
				continue;
			}

			else {
				printf("Error, you didn't enter \"c\" or \"f\" \n");
				counter = 0;
				continue;
			}
			counter = 0;

		}
		printf("%s\n", variable3.incrypted_selected_movie[0]);
	}

	return variable3;
}
