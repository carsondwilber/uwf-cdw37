/*
 * hashTable.h
 *
 *  Created on: Jan 29, 2018
 *      Author: carso
 */

#include <stdlib.h>

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

struct Word_st {
	char * word;
	struct Word_st * next;
};

struct HashTable_st {
	int numberOfBuckets;
	struct Word_st ** buckets;
	int (*hash)(char *, int, int);
};

typedef struct Word_st Word_t;
typedef struct HashTable_st HashTable_t;

HashTable_t * createHashTable(int numBuckets, int (*hashFunction)(char *, int, int));

Word_t * createEntry(char * word, int wordLength);

void addWord(HashTable_t * table, char * word, int wordLength);

int addEntry(HashTable_t * table, Word_t * entry);

Word_t * findEntry(HashTable_t * table, char * word, int wordLength);

int entryExists(HashTable_t * table, char * word, int wordLength);

int cumulativeCharFactorHash(char * word, int wordLength, int modulus);

int simpleSumHash(char * word, int wordLength, int modulus);

int constantCharFactorHash(char * word, int wordLength, int modulus);

void calculateStatistics(HashTable_t * table, double * alpha, double * empty, int * longest, int * average, double * lonely);

#endif /* HASHTABLE_H_ */
