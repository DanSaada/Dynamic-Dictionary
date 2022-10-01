/***********
 * @author Dan Saada
 * @version ex_5
 * @since 06/12/2021
***********/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define SPACE " "
#define COMMA ","
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET_COLOR "\033[0m"

typedef struct Word {
	char** translations;
	struct Word* next;
} Word;

typedef struct {
	char** languages;
	int numOfLanguages;
	Word* wordList;
} Dictionary;

/******************
* Function Name: getUserInput
* Input: None
* Output: char* userInput
* Function Operation: This function is an auxiliary function that returns a pointer to a string that the user enters.
					  The function is being used in two other functions called "createNewDic" & "createWord".
* ******************/
char* getUserInput() {
	int countLetters = 0;
	char c;
	//temporary variable used to prevent memory leakage
	char* tmp = NULL;
	//a pointer to a string scaned from the user that contains a languages or a word.
	//sizeof(char) + 1 : the +1 is for preventing buffer overrun.
	char* userInput = (char*)malloc(sizeof(char) + 1);
	if (userInput == NULL) {
		printf("fail user input");
		free(userInput);
	}
	printf(YELLOW);
	scanf(" %c", &c);
	printf(RESET_COLOR);

	while (c != '\n') {
		//prevent dereferencing
		if (userInput) {
			userInput[countLetters] = c;
		}
		countLetters++;
		scanf("%c", &c);
		tmp = (char*)realloc(userInput, ((sizeof(char) * (countLetters + 1))));
		if (tmp == NULL) {
			printf("fail");
			free(tmp);
		}
		else {
			userInput = tmp;
		}
	}
	//putting "\0" at the end of the string
	if (userInput) {
		userInput[countLetters] = '\0';
	}
	return userInput;
}

/******************
* Function Name: getStr
* Input: char* newUserInput, int* counter, char** str, char* choose
* Output: char** strLanguages
* Function Operation: This function is an auxiliary function that returns a pointer to an array of pointers.
					  The function is being called in two other function called "createNewDic" & "createWord".
					  In "createNewDic" function: this auxiliary function helps return a pointer to an array of
					  char*'s, called strLanguages.
					  In "createWord" function: this auxiliary function helps return a pointer to an array of
					  char*'s, called strWords.
* ******************/
char** getStr(char* newUserInput, int* counter, char** str, char* choose) {
	//temporary variable used to prevent memory leakage
	char** tmp = NULL;
	//token= a pointer that points each time to a string in the userInput array.
	char* token;
	//Divide the string into parts, with each part contains a different "choose".
	token = strtok(newUserInput, COMMA);
	/*str is a pointer to an array of char *'s(of pointers). Meaning for each organ in str we allocate the correct
	 number of chars. So in each iteration of the while loop we allocate exactly the number of chars of the currrent
	 token plus 1 to "choose", and change the memory alloction of str in accordance.
	 */
	while (token != NULL) {
		choose = NULL;
		choose = (char*)malloc(sizeof(char) * (strlen(token) + 1));
		if (choose == NULL) {
			printf("fail language ");
		}
		//copying token into choose.
		if (choose != NULL) {
			strcpy(choose, token);
		}
		//re-allocate memory for str if needed
		if ((*counter) >= 1) {
			tmp = (char**)realloc(str, ((sizeof(char*)) * ((*counter) + 1)));
			if (tmp == NULL) {
				printf("fail");
				free(tmp);
			}
			else {
				str = tmp;
			}
		}
		//putting the "choices" in a specific order.
		str[(*counter)] = choose;
		token = strtok(NULL, COMMA);
		(*counter)++;
	}
	return str;
}

/******************
* Function Name: creatNewDic
* Input: None
* Output: Dictionary
* Function Operation: This function is an auxiliary function that executes placement commands between the input entered
					  by the user and all dictionaries sub-parts, and returns a dictionary
					  When creating a new dictionary the user will insert a comma-separated string that defines the
					  source language (the first word in the string), and the additional languages ​​that the dictionary
					  supports.
					  The function uses 2 other auxiliary function called "getUserInput" & "getStr".
* ******************/
Dictionary createNewDic() {
	int counter = 0;
	//"**" because strLanguages is a pointer to an array of char *'s 
	char** strLanguages = (char**)malloc(sizeof(char*));
	if (strLanguages == NULL) {
		printf("fail creat table of languages");
		free(strLanguages);
	}
	//a pointer that points each time to a specific language.
	char* languageChoose = NULL;
	printf("Define a new dictionary:\n");
	//getting the user input.
	char* newUserInput = getUserInput();
	//getting the pointer to the array of pointers.
	char** newStrLanguages = getStr(newUserInput, &counter, strLanguages, languageChoose);
	free(newUserInput);
	free(languageChoose);
	//a struct that the function returns with all the definitions of a specific dictionary.
	Dictionary dictionary;
	dictionary.languages = newStrLanguages;
	dictionary.numOfLanguages = counter;
	dictionary.wordList = NULL;
	return dictionary;
}

/******************
* Function Name: addNewDict
* Input: Dictionary* dictionaries, int numOfDictionaries
* Output: Dictionary* dictionaries || tmpDictionaries (Depends on the scenario)
* Function Operation: This function is used by the user to add new dictionaries to the program.
*					  The function calls an auxiliary function called "creatNewDic"
*					  When the number of dictionaries is 1 the function will allocate a Dictionary struct variable 
					  type using malloc. Else (number of dictionaries > 1), the function will re-allocate the
					  allocation of the memory.
					  Finally, the function will print a message on the monitor that updates whether the dictionary
					  creation was successful or failed.
******************/
Dictionary* addNewDict(Dictionary* dictionaries, int numOfDictionaries) {
	//temporary variable used to prevent memory leakage.
	Dictionary* tmpDictionaries;
	//creat the dictionary.
	Dictionary dict = createNewDic();
	// first dictionary
	if (numOfDictionaries == 1) {
		dictionaries = (Dictionary*)malloc(sizeof(Dictionary));
		//allocation failed
		if (dictionaries == NULL) {
			printf("The creation of the dictionary has failed!");
			free(dictionaries);
			return dictionaries;
		}
		//Allocation was successful
		else {
			dictionaries[numOfDictionaries - 1] = dict;
			return dictionaries;
		}
	}
	//more then one dictionaries
	else {
		tmpDictionaries = (Dictionary*)realloc(dictionaries, (sizeof(Dictionary) * numOfDictionaries));
		//allocation failed
		if (tmpDictionaries == NULL) {
			numOfDictionaries--;
			printf("The creation of the dictionary has failed!");
			return dictionaries;
		}
		//allocation was successful
		else {
			tmpDictionaries[numOfDictionaries - 1] = dict;
			return tmpDictionaries;
		}
	}
}

/******************
* Function Name: printDic
* Input: Dictionary* dictionaries, int numOfDictionaries, int* dicChoose
* Output: None
* Function Operation: This function is an auxiliary function that prints a numbered list of strings.
* ******************/
void printDic(Dictionary* dictionaries, int numOfDictionaries, int* dicChoose) {
	printf("Choose a dictionary:\n");
	//runs on the dictionaries
	for (int i = 0; i < (numOfDictionaries); i++){
		printf("%d. ", i + 1);
		//runs on the languages
		for (int j = 0; j < dictionaries[i].numOfLanguages; j++) {
			printf("%s", dictionaries[i].languages[j]);
			if (j != dictionaries[i].numOfLanguages - 1) {
				printf(COMMA);
			}
		}
		printf("\n");
	}
	printf(YELLOW);
	scanf(" %d", dicChoose);
	printf(RESET_COLOR);
	do {
		//checking user mistakes in dictionary choose input.
		if ((*dicChoose) > numOfDictionaries || (*dicChoose) < 1) {
			printf("fail user selction of dic\n");
			scanf(" %d", dicChoose);
		}
	} while ((*dicChoose) > numOfDictionaries || (*dicChoose) < 1);
	//match the value of dicChoose in the user selection list, to the location of the dicChoose in the dictionaries. 
	((*dicChoose)--);
}

/******************
* Function Name: createWord
* Input: Dictionary* dictionaries, int numOfDictionaries, int* dicChoose
* Output: Word
* Function Operation: This function is used by the user to add words to existing dictionaries.
*					  The user chooses which dictionary he wants to add a word to, Then inserts the word and its
					  translation (when separated by commas) in the order in which the languages ​​are arranged in the
					  specific dictionary chosen. if the user made a mistake, such as forgot to provide one of the
					  translations of the dictionary he choose, the program will cancel the operation and let him know.
* ******************/
Word createWord(Dictionary* dictionaries, int numOfDictionaries, int* dicChoose, int* isTranslationInput) {
	int counter = 0;
	//allocate a pointer to an array of char's*
	char** strWords = (char**)malloc(sizeof(char*));
	if (strWords == NULL) {
		printf("fail");
		free(strWords);
	}
	char* wordChoose = NULL;
	//printing all dictionary's options and getting the user choice .
	printDic(dictionaries, numOfDictionaries, dicChoose);
	//a message asking the user to enter words in the requested dictionary
	printf("Enter a word in ");
	for (int i = 0; i < dictionaries[(*dicChoose)].numOfLanguages; i++) {
		printf("%s", dictionaries[(*dicChoose)].languages[i]);
		if (i != dictionaries[(*dicChoose)].numOfLanguages - 1) {
			printf(COMMA);
		}
	}
	printf(":\n");
	//scan the user word and transaltions.
	//a pointer to a string scaned from the user that contains the word ​​and its translations.
	char* newUserInput = getUserInput();
	char** newStrWords = getStr(newUserInput, &counter, strWords, wordChoose);
	Word word;
	//checking that the input is correct.
	if (dictionaries[(*dicChoose)].numOfLanguages != counter) {
		printf(RED);
		printf("The addition of the word has failed!\n"
			"Please insert number of translations as the number of languages in the choosen dictionary.\n");
		printf(RESET_COLOR);
		(*isTranslationInput = 1);
		return;
	}

	free(newUserInput);
	//filling all the data of a specific word.
	word.translations = newStrWords;
	word.next = NULL;
	return word;
}

void addWord(int* numOfWords, Dictionary* dictionaries, int numOfDictionaries, int* dicChoose) {
	int isTranslationInput = 0;
	Word* words = NULL;
	Word* tmpWord = NULL;
	numOfWords++;
	words = (Word*)malloc(sizeof(Word));
	// to enter the loop fo the first word sinerio
	words->next = NULL;
	words->translations = NULL;
	//check if allocation worked
	if (words == NULL) {
		numOfWords--;
		printf(RED);
		printf("The addition of the word has failed!\n");
		printf(RESET_COLOR);
		free(words);
		return;
	}
	else {
		*words = createWord(dictionaries, numOfDictionaries, dicChoose, &isTranslationInput);
		//fail in user input
		if (isTranslationInput == 1) { return; }
		//dictionary is empty
		if (dictionaries[(*dicChoose)].wordList == NULL) {
			dictionaries[(*dicChoose)].wordList = words;
		}
		else {
			//tmpWord represent a list of words of the choosen dictionary
			tmpWord = dictionaries[(*dicChoose)].wordList;
			//checking if the word already exists in a specific dictionary.
			while (tmpWord != NULL) {
				if (strcmp(tmpWord->translations[0], words->translations[0]) == 0) {
					printf("This word is already exists in this dictionary, do you want to revalue its translations?\n");
					//giving the user a chance to regret.
					int answer = getAnswer();
					if (answer == 0) {
						printf(PURPLE);
						printf("The addition of the word has been canceled.\n");
						printf(RESET_COLOR);
						return;
					}
					else {
						//replace old translations with new ones.
						for (int i = 1; i < dictionaries[(*dicChoose)].numOfLanguages; i++) {
							tmpWord->translations[i] = words->translations[i];
						}
					}
				}
				tmpWord = tmpWord->next;
			}
			tmpWord = words;
		}
		printf(GREEN);
		printf("The word has been added successfully!\n");
		printf(RESET_COLOR);
	}
}

/******************
* Function Name: getAnswer
* Input: None
* Output: 1 for confirmation
		  0 for cancelation
* Function Operation: This function is an auxiliary function that allows the user to regret an action he wanted to do.
					  The function will return to the user a message that the action has been canceled if he so chooses.
* ******************/
int getAnswer() {
	char yes = 'y';
	printf("Are you sure? (y/n)\n");
	char* answer = getUserInput();
	if (yes == (*answer)) {
		free(answer);
		return 1;
	}
	else {
		free(answer);
		return 0;
	}
}

/******************
* Function Name: freeStringTable
* Input: char** table, int numOfRows
* Output: None
* Function Operation: This function is an auxiliary function that Deletes a word and its translations in a specific
					  dictionary.
* ******************/
void freeStringTable(char** table, int numOfRows) {
	if (table != NULL) {
		for (int i = 0; i < numOfRows; i++) {
			free(table[i]);
			table[i] = NULL;
		}
		free(table);
		table = NULL;
	}
}

/******************
* Function Name: freeWord
* Input: Word* wordToFree, Dictionary* dictionaries, int numOfLanguages
* Output: Word* wordToFree
* Function Operation: This function is an auxiliary function that Deletes a word in a dictionary.
*					  The function uses an other auxiliary function called "freeStringTable"
* ******************/
Word* freeWord(Word* wordToFree, Dictionary* dictionaries, int numOfLanguages) {
	if (wordToFree != NULL) {
		// free all the translations (each and every one of them)
		freeStringTable(wordToFree->translations, numOfLanguages);
		// free word*
		free(wordToFree);
		wordToFree = NULL;
		return wordToFree;
	}
	return wordToFree;
}

/******************
* Function Name: freeWord2
* Input: Word* word, int numOfTranslations
* Output: None
* Function Operation: This function is an auxiliary function that Deletes a word in a dictionary when we want to
					  delete a dictionary.
*					  The function uses recursion and calls itself.
* ******************/
void freeWord2(Word* word, int numOfTranslations) {
	//we went to the last word and we go back to whom called us
	if (word == NULL) {
		return;
	}
	//recursion call
	freeWord2(word->next, numOfTranslations);
	//delete all the translatoins
	for (int i = 0; i < numOfTranslations; i++) {
		free(word->translations[i]);
		word->translations[i] = NULL;
	}
	//delete the pointer of the translations
	free(word->translations);
	word->translations = NULL;
	//delete the pointer it self,
	free(word);
	word = NULL;
}

/******************
* Function Name: freeDict
* Input: Dictionary* dictionaries, int numOfDictionaries, int* dicChoose
* Output: Dictionary* dictionaries / newDicts (Depends on the scenario)
* Function Operation: This function is an auxiliary function used to delete the internal parts of the dictionary
					  among them : languages, numOfLanguages, wordList.
					   The function is divided into cases:
					   * dictionary is in the middle
					   * dictionary is first or last
					   The function uses 2 other auxiliary function called "freeWord2" & "freeStringTable"
* ******************/
Dictionary* freeDict(Dictionary* dictionaries, int numOfDictionaries, int* dicChoose) {

	Dictionary* newDicts = NULL;
	// case 1 - in the middle
	if (((*dicChoose) != 0) && ((*dicChoose) < (numOfDictionaries - 1))) {
		// free word list
			freeWord2(dictionaries[(*dicChoose)].wordList, dictionaries[(*dicChoose)].numOfLanguages);
			//free the translations
			freeStringTable(dictionaries[(*dicChoose)].languages, dictionaries[(*dicChoose)].numOfLanguages);
			for (int i = (*dicChoose); i < ((numOfDictionaries) - 1); i++) {
				dictionaries[i] = dictionaries[i + 1];
			}
			newDicts = (Dictionary*)realloc(dictionaries, ((numOfDictionaries) - 1) * sizeof(Dictionary));
	}
	// case 2 - first or last
	else if (((*dicChoose) == (numOfDictionaries - 1)) || ((*dicChoose) == 0)) {
		// free word list
		freeWord2(dictionaries[(*dicChoose)].wordList, dictionaries[(*dicChoose)].numOfLanguages);
		//free the translations
		freeStringTable(dictionaries[(*dicChoose)].languages, dictionaries[(*dicChoose)].numOfLanguages);
		for (int i = (*dicChoose); i < ((numOfDictionaries)-1); i++) {
			dictionaries[i] = dictionaries[i + 1];
		}
		newDicts = (Dictionary*)realloc(dictionaries, (numOfDictionaries - 1) * sizeof(Dictionary));
	}
	if (newDicts != NULL) {
		return newDicts;
	}
	else if (newDicts == NULL && numOfDictionaries == 1) {
		return NULL;
	}
	else {
		// error
		return dictionaries;
	}
}

/******************
* Function Name: deleteWord
* Input: Dictionary* dictionaries, int numOfDictionaries, int* dicChoose
* Output: None
* Function Operation: This function deletes a word in a dictionary of the user's choice.
					   The function uses 2 other auxiliary function called "getUserInput" & "getStr".
* ******************/
void deleteWord(Dictionary* dictionaries, int numOfDictionaries, int* dicChoose) {
	int answer = 0;
	//printing all dictionary's options.
	printDic(dictionaries, numOfDictionaries, dicChoose);
	printf("Enter a word in %s:\n", dictionaries[*dicChoose].languages[0]);
	//a pointer to a string scaned from the user that contains the languages ​​of a dictionary.
	char* newUserInput = getUserInput();
	//giving the user a chance to regret.
	answer = getAnswer();
	if (answer == 0) {
		printf(PURPLE);
		printf("The deletion of the word has been canceled.\n");
		printf(RESET_COLOR);
		free(newUserInput);
		return;
	}
	//a pointer to the begining of the list.
	Word* current = dictionaries[*dicChoose].wordList;
	//a pointer who will be pointing on the word before the one we want to delete.
	Word* prev = NULL;

	//checking if the word that the user entered exist.
	if (current != NULL && strcmp(current->translations[0], newUserInput) == 0) {
		dictionaries[*dicChoose].wordList = current->next;
		current = freeWord(current, dictionaries, dictionaries[*dicChoose].numOfLanguages);
		if (current != NULL) {
			printf("The deletion of the word has failed!\n");
		}
		else {
			printf(GREEN);
			printf("The word has been deleted successfully!\n");
			printf(RESET_COLOR);

		}
		free(newUserInput);
		return;
	}

	//promoting pointers to go through the entire list
	while (current != NULL && strcmp(current->translations[0], newUserInput) != 0) {
		prev = current;
		current = current->next;
	}
	//not found
	if (current == NULL) {
		printf("The deletion of the word has failed!\n");
		free(newUserInput);
		return;
	}
	// Unlink the Word* from linked list
	prev->next = current->next;
	//free the word
	current = freeWord(current, dictionaries, dictionaries[(*dicChoose)].numOfLanguages);
	if (current != NULL) {
		printf("The deletion of the word has failed!\n");
	}
	else {
		printf(GREEN);
		printf("The word has been deleted successfully!\n");
		printf(RESET_COLOR);
	}
}

/******************
* Function Name: printWord
* Input: Word* wordToPrint, int* dicChoose, Dictionary* dictionaries
* Output: None
* Function Operation: This function is an auxiliary function that prints the translations to the user.
* ******************/
void printWord(Word* wordToPrint, int* dicChoose, Dictionary* dictionaries) {
	printf("The translations are:\n");
	for (int i = 1; i < dictionaries[*dicChoose].numOfLanguages; i++) {
		
		printf("%s: ", dictionaries[*dicChoose].languages[i]);
		printf(CYAN);
		
		printf("%s", wordToPrint->translations[i]);
		printf(RESET_COLOR);
		if (i != dictionaries[*dicChoose].numOfLanguages - 1) {
			printf(COMMA);
			printf(SPACE);
		}
	}
	printf("\n");
}

/******************
* Function Name: findWord
* Input: Dictionary* dictionaries, int numOfDictionaries, int* dicChoose
* Output: 1 if word is found
		  0 if the word is not found
* Function Operation: This function is a used to find a word in the dictionary and prints the translations of that word
					  with the languages ​​that support the specific dictionary.
					   The function uses 2 other auxiliary function called "printDic" & "printWord".
					   The function is divided into cases:
					   * no words in the dictionaries
					   * only one word in dict
					   * more then one word in dict
					   * the word is in the end of the list
* ******************/
int findWord(Dictionary* dictionaries, int numOfDictionaries, int* dicChoose) {
	int countLetters = 0;
	//printing all dictionary's options.
	printDic(dictionaries, numOfDictionaries, dicChoose);
	printf("Enter a word in %s:\n", dictionaries[*dicChoose].languages[0]);
	//a pointer to a string scaned from the user that contains the languages ​​of a dictionary.
	char* newUserInput = getUserInput();
	Word* currentWord = NULL;
	currentWord = dictionaries[*dicChoose].wordList;
	
	//case 1 - no words in the dictionaries
	if (currentWord == NULL) {
		printf(RED);
		printf("There are no translations for \"%s\" in this dictionary.\n", newUserInput);
		printf(RESET_COLOR);
		free(newUserInput);
		return 0;
	}

	// case 2 - only one word in dict
	if (currentWord->next == NULL) {
		 //checking if current word is the word i am looking for
		if (strcmp(newUserInput, currentWord->translations[0]) == 0) {
			printWord(currentWord, dicChoose, dictionaries);
			free(newUserInput);
			return 1;
		}
		else {
			printf(RED);
			printf("There are no translations for \"%s\" in this dictionary.\n", newUserInput);
			printf(RESET_COLOR);
			free(newUserInput);
			return 0;
		}
	}
	else {
		 //case 3 - more then one word in dict
		while (currentWord->next != NULL) {
			// checking if current word is the word i am looking for
			if (strcmp(newUserInput, currentWord->translations[0]) == 0) {
				printWord(currentWord, dicChoose, dictionaries);
				free(newUserInput);
				return 1;
			}
			// go to the next word
			currentWord = currentWord->next;
		}
		// case 4 - last round
		if (strcmp(newUserInput, currentWord->translations[0]) == 0) {
			printWord(currentWord, dicChoose, dictionaries);
			free(newUserInput);
			return 1;
		}
		printf(RED);
		printf("There are no translations for \"%s\" in this dictionary.\n", newUserInput);
		printf(RESET_COLOR);
	}
	free(newUserInput);
	return 0;
}

/******************
* Function Name: deleteDic
* Input: Dictionary* dictionaries, int numOfDictionaries, int* dicChoose
* Output: None
* Function Operation: This function deletes a specific dictionary from the existing dictionary set
					   The function uses 2 other auxiliary function called "getAnswer" & "freeDict".
					   After entering a dictionary to delete, the user will receive a message from the program that
					   allows him to regret his action.
					   If the user do regret, the program will print a message that the delete operation has canceled.
* ******************/
Dictionary* deleteDic(Dictionary* dictionaries, int* numOfDictionaries, int* dicChoose) {
	//printing all dictionary's options.
	printDic(dictionaries, (*numOfDictionaries), dicChoose);
	//verify that the user does not delete a dictionary by mistake and create an option to undo the action.
	int answer = getAnswer();
	if (answer == 0) {
		printf(PURPLE);
		printf("The deletion of the dictionary has been canceled.\n");
		printf(RESET_COLOR);
		return dictionaries;
	}
	Dictionary* newDicts = freeDict(dictionaries, (*numOfDictionaries), dicChoose);
	//checking if the dictionary has been canceld and and inform the user.
	if ((newDicts == NULL && (*numOfDictionaries) == 1) || (newDicts != NULL)) {
		printf(GREEN);
		printf("The dictionary has been deleted successfully!\n");
		printf(RESET_COLOR);
		(*numOfDictionaries)--;
	}
	return newDicts;
}

/******************
* Function Name: exitProgram 
* Input: Dictionary* dictionaries, int numOfDictionaries, int* dicChoose
* Output: None
* Function Operation: This function exits the program and deletes all the dictionaries and memory allocations in
					  the program.
******************/
void exitProgram(Dictionary* dictionaries, int numOfDictionaries, int* dicChoose) {
	int tmp = numOfDictionaries - 1;
	//deleting all dictionaries from the last to the first
	for(int i = numOfDictionaries - 1; i >= 0; i--){
		freeDict(dictionaries, i, &tmp);
		tmp--;
	}
	// free the pointer to the dictionaries
	if (dictionaries != NULL) {
		free(dictionaries);
		dictionaries = NULL;
	}

	printf("Bye!");
}

/******************
* Function Name: printMenu
* Input: None
* Output: None
* Function Operation: This function prints the menu.
******************/
void printMenu() {
	printf("Welcome to the dictionaries manager!\nChoose an option:\n"
		"1. Create a new dictionary.\n"
		"2. Add a word to a dictionary.\n"
		"3. Delete a word from a dictionary.\n"
		"4. Find a word in a dictionary.\n"
		"5. Delete a dictionary.\n"
		"6. Exit.\n");
}

/******************
* Function Name: menu
* Input: None
* Output: None
* Function Operation: This function is used by the user to navigate a convenient visually menu, between all the
					  functions that the program contains.
					  According to the user's choice, the requested task will be performed.
					  When a task is completed, the menu will be reprinted and the user could select again.
					  If the user presses 6, the program will end.
					  If the user presses an option that doesn't appear in the menu, the program will print an
					  error message, and the menu will be reprinted.
******************/
void menu() {
	int dicChoose = 0;
	int x = 0, numOfDictionaries = 0, numOfWords = 0;
	Dictionary* dictionaries = NULL;

	do {
		printMenu();
		//get user choice.
		printf(YELLOW);
		scanf("%d", &x);
		printf(RESET_COLOR);
		//no existing dictionaries yet.
		if (x > 1 && x < 6 && numOfDictionaries == 0) {
			printf(RED);
			printf("This option is not available right now, try again:\n");
			printf(RESET_COLOR);
			continue;
		}
		switch (x) {

		// ADD DIC
		case 1:
			numOfDictionaries++;
			dictionaries = addNewDict(dictionaries, numOfDictionaries);
			if (dictionaries != NULL) {
				printf(GREEN);
				printf("The dictionary has been created successfully!\n");
				printf(RESET_COLOR);
			}
			break;
		// ADD WORD
		case 2:
			addWord(&numOfWords, dictionaries, numOfDictionaries, &dicChoose);
			break;
		// DELETE WORD
		case 3:
			deleteWord(dictionaries, numOfDictionaries, &dicChoose);
			break;
		// FIND WORD
		case 4:
			findWord(dictionaries, numOfDictionaries, &dicChoose);
			break;
		// DELETE DIC
		case 5:
			dictionaries = deleteDic(dictionaries, &numOfDictionaries, &dicChoose);	
			break;
		// EXIT
		case 6:
			exitProgram(dictionaries, numOfDictionaries, &dicChoose);
			dictionaries = NULL;
			break;

		default:
			printf(RED);
			printf("Wrong option, try again:\n");
			printf(RESET_COLOR);
			break;
		}
	} while (x != 6);
}

/******************
* Function Name: main
* Input: None
* Output: None
* Function Operation: This function initializes the menu function.
******************/
void main() {
	menu();
}