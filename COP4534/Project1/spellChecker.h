/*
 * spellChecker.h
 *
 *  Created on: Jan 29, 2018
 *      Author: carso
 */

#include "hashTable.h"

#ifndef SPELLCHECKER_H_
#define SPELLCHECKER_H_

void stripPunctuation(char * word, int * wordLength);

void toLowercase(char * word, int wordLength);

int isAlphabetic(char * word, int wordLength);

Word_t * getSuggestions(HashTable_t * hashTable, char * word, int wordLength);

Word_t * missingLetter(HashTable_t * hashTable, char * word, int wordLength, Word_t ** current);

Word_t * extraLetter(HashTable_t * hashTable, char * word, int wordLength, Word_t ** current);

Word_t * swappedLetters(HashTable_t * hashTable, char * word, int wordLength, Word_t ** current);

Word_t * joinSuggestions(Word_t * head, Word_t * tail);

#endif /* SPELLCHECKER_H_ */
