/*
 * main.c
 *
 *  Created on: Jan 30, 2018
 *      Author: carso
 */

#include "hashTable.h"
#include "spellChecker.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int BUFFER_SIZE = 256;

HashTable_t * hashTable;

int main(int argc, char ** argv) {
	hashTable = createHashTable(60000, constantCharFactorHash);

	printf("Building dictionary...\n");

	FILE* dictionary = fopen("Dictionary.txt", "r");
	char buffer[BUFFER_SIZE];
	int wordLength;
	int words = 0;

	while (fgets(buffer, BUFFER_SIZE, dictionary)) {
		wordLength = strlen(buffer) - 2;

		buffer[wordLength] = '\0';

		addWord(hashTable, buffer, wordLength);

		words++;
	}

	fclose(dictionary);

	printf("Dictionary built with %d words.\n", words);

	double alpha, empty, lonely;
	int longest, average;

	calculateStatistics(hashTable, &alpha, &empty, &longest, &average, &lonely);

	printf("Load factor (alpha): %.2f\n", alpha);
	printf("%% empty buckets: %.2f%%\n", empty * 100.0);
	printf("Longest chain: %d\n", longest);
	printf("Average chain: %d\n", average);
	printf("%% lonely buckets: %.2f%%\n", lonely * 100.0);
	printf("\n\n");

	FILE* spellCheck = fopen("testTextFile.txt", "r");
	Word_t * suggestions;
	Word_t * next;
	char * token;
	int editedLength;
	char * edited;

	while (fgets(buffer, BUFFER_SIZE, spellCheck)) {
		token = strtok(buffer, " ");

		do {
			editedLength = strlen(token);
			edited = (char *)malloc(sizeof(char) * editedLength);
			strcpy(edited, token);
			stripPunctuation(edited, &editedLength);
			toLowercase(edited, strlen(edited));

			if (entryExists(hashTable, edited, editedLength)) {
				suggestions = (Word_t *)0;
				printf("%s ", token);
			} else {
				edited[0] = (char)toupper((int) edited[0]);

				if (entryExists(hashTable, edited, editedLength)) {
					suggestions = (Word_t *)0;
					printf("%s ", token);
				} else {
					suggestions = getSuggestions(hashTable, token, strlen(token));
					printf("[");

					if (suggestions) {
						printf("'%s': %s", token, suggestions->word);

						while (suggestions->next) {
							next = suggestions->next;

							free(suggestions);

							suggestions = next;

							printf(", %s", suggestions->word);
						}
					} else {
						printf("%s?", token);
					}

					printf("] ");
				}
			}

			free(edited);
		} while ((token = strtok(0, " ")));
	}
}


