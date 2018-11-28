/*
 * spellChecker.c
 *
 *  Created on: Jan 29, 2018
 *      Author: carso
 */

#include "spellChecker.h"
#include "hashTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void stripPunctuation(char * word, int * wordLength) {
	int i;
	int offset = 0;

	for (i = 0; i + offset < *wordLength; ++i) {
		if (word[i] == '.' || word[i] == ',') {
			offset++;
		}

		word[i] = word[i + offset];
	}

	(*wordLength) -= offset;
	word[*wordLength] = '\0';
}

void toLowercase(char * word, int wordLength) {
	int i;

	for (i = 0; i < wordLength; ++i) {
		word[i] = (char)tolower((int) word[i]);
	}
}

int isAlphabetic(char * word, int wordLength) {
	int i;

	for (i = 0; i < wordLength; ++i) {
		if (word[i] < 'A' || (word[i] > 'Z' && word[i] < 'a') || word[i] > 'z') {
			return 0;
		}
	}

	return 1;
}

Word_t * joinSuggestions(Word_t * head, Word_t * tail) {
	if (tail) {
		Word_t * appendTo = head;

		while (appendTo->next) {
			appendTo = appendTo->next;
		}

		appendTo->next = tail;
	}

	return head;
}

Word_t * getSuggestions(HashTable_t * hashTable, char * word, int wordLength) {
	if (!word || wordLength < 1) {
		printf("Error in suggest(char *, int): Word is empty!\n");
		return (Word_t *)0;
	}

	stripPunctuation(word, &wordLength);

	if (!isAlphabetic(word, wordLength)) {
		printf("Error in suggest(char *, int): Word is not even alphabetic!\n");
		return (Word_t *)0;
	}

	Word_t * ret = (Word_t *)0;

	ret = missingLetter(hashTable, word, wordLength, &ret);

	ret = extraLetter(hashTable, word, wordLength, &ret);

	ret = swappedLetters(hashTable, word, wordLength, &ret);

	return ret;
}

Word_t * missingLetter(HashTable_t * hashTable, char * word, int wordLength, Word_t ** current) {
	Word_t * suggestions = (Word_t *)0;
	int i, x;
	char c;
	int capitalized = (word[0] >= 'A' && word[0] <= 'Z');

	for (i = 0; i <= wordLength; ++i) {
		for (c = 'a'; c < 'z'; c++) {
			char * guessWord = (char *)malloc(sizeof(char) * (wordLength + 2));

			for (x = 0; x < i; ++x) {
				guessWord[x] = word[x];
			}

			guessWord[x] = c;

			for (x++; x <= wordLength; ++x) {
				guessWord[x] = word[x - 1];
			}

			guessWord[wordLength + 1] = '\0';

			toLowercase(guessWord, wordLength + 1);

			int exists = entryExists(hashTable, guessWord, wordLength + 1);

			if (exists) {
				Word_t * suggestion = createEntry(guessWord, wordLength + 1);

				if (suggestions) {
					suggestion->next = suggestions;
				}

				suggestions = suggestion;
			}

			// check for capitalized word too in case, for example, typed "alabam" instead of "Alabam"
			if (capitalized || !exists) {
				guessWord[0] = (char)toupper((int) guessWord[0]);

				if (entryExists(hashTable, guessWord, wordLength - 1)) {
					Word_t * suggestion = createEntry(guessWord, wordLength - 1);

					if (suggestions) {
						suggestion->next = suggestions;
					}

					suggestions = suggestion;
				}
			}
		}
	}

	return (suggestions ? joinSuggestions(suggestions, *current) : *current);
}

Word_t * extraLetter(HashTable_t * hashTable, char * word, int wordLength, Word_t ** current) {
	Word_t * suggestions = (Word_t *)0;
	int i, x;
	int capitalized = (word[0] >= 'A' && word[0] <= 'Z');

	for (i = 0; i < wordLength; ++i) {
		char * guessWord = (char *)malloc(sizeof(char) * (wordLength));

		for (x = 0; x < i; ++x) {
			guessWord[x] = word[x];
		}

		for (x++; x < wordLength; ++x) {
			guessWord[x - 1] = word[x];
		}

		guessWord[wordLength - 1] = '\0';

		toLowercase(guessWord, wordLength - 1);

		int exists = entryExists(hashTable, guessWord, wordLength - 1);

		if (exists) {
			Word_t * suggestion = createEntry(guessWord, wordLength - 1);

			if (suggestions) {
				suggestion->next = suggestions;
			}

			suggestions = suggestion;
		}

		// check for capitalized word too in case, for example, typed "alabamaa" instead of "Alabamaa"
		if (capitalized || !exists) {
			guessWord[0] = (char)toupper((int) guessWord[0]);

			if (entryExists(hashTable, guessWord, wordLength - 1)) {
				Word_t * suggestion = createEntry(guessWord, wordLength - 1);

				if (suggestions) {
					suggestion->next = suggestions;
				}

				suggestions = suggestion;
			}
		}
	}

	return (suggestions ? joinSuggestions(suggestions, *current) : *current);
}

Word_t * swappedLetters(HashTable_t * hashTable, char * word, int wordLength, Word_t ** current) {
	Word_t * suggestions = (Word_t *)0;
	int i, x;
	int capitalized = (word[0] >= 'A' && word[0] <= 'Z');

	for (i = 0; i < wordLength - 1; ++i) {
		char * guessWord = (char *)malloc(sizeof(char) * wordLength + 1);

		for (x = 0; x < i; ++x) {
			guessWord[x] = word[x];
		}

		guessWord[x] = word[x + 1];
		guessWord[x + 1] = word[x];

		for (x += 2; x < wordLength; ++x) {
			guessWord[x] = word[x];
		}

		guessWord[wordLength] = '\0';

		toLowercase(guessWord, wordLength);

		int exists = entryExists(hashTable, guessWord, wordLength);

		if (exists) {
			Word_t * suggestion = createEntry(guessWord, wordLength);

			if (suggestions) {
				suggestion->next = suggestions;
			}

			suggestions = suggestion;
		}

		// check for capitalized word too in case, for example, typed "alabmaa" instead of "Alabmaa"
		if (capitalized || !exists) {
			guessWord[0] = (char)toupper((int) guessWord[0]);

			if (entryExists(hashTable, guessWord, wordLength)) {
				Word_t * suggestion = createEntry(guessWord, wordLength);

				if (suggestions) {
					suggestion->next = suggestions;
				}

				suggestions = suggestion;
			}
		}
	}

	return (suggestions ? joinSuggestions(suggestions, *current) : *current);
}
