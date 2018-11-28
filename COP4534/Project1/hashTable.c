/*
 * hashTable.c
 *
 *  Created on: Jan 29, 2018
 *      Author: carso
 */

#include "hashTable.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Creates an entry using the given word and its length.
 */
Word_t * createEntry(char * word, int wordLength) {
	Word_t * entry = (Word_t *)malloc(sizeof(Word_t));
	entry->next = (Word_t *)0;
	entry->word = (char *)malloc(sizeof(char) * (wordLength + 1));

	int i;

	for (i = 0; i < wordLength; ++i) {
		entry->word[i] = word[i];
	}

	entry->word[i] = '\0';

	return entry;
}

/*
 * Convenience function that combines createEntry and addEntry using the given word and table.
 */
void addWord(HashTable_t * table, char * word, int wordLength) {
	if (addEntry(table, createEntry(word, wordLength)) == 0) {
		printf("Error in addWord(HashTable_t *, char *, int): Failed to add entry to table!\n");
	}
}

/*
 * Adds the given entry to the given table, using the table's hash function to select a bucket.
 *
 * Returns 1 if succeeded, 0 if table, chain, or entry are null.
 */
int addEntry(HashTable_t * table, Word_t * entry) {
	if (!table) {
		printf("Error in addEntry(HashTable_t, BucketEntry_t): Table not defined!\n");
		return 0;
	}

	if (!entry) {
		printf("Error in addEntry(HashTable_t, BucketEntry_t): Entry not defined!\n");
		return 0;
	}

	if (!table->buckets) {
		printf("Error in addEntry(HashTable_t, BucketEntry_t): Table's buckets not defined!\n");
		return 0;
	}

	// hash entry's word using the table's hash function and modulus
	int hashValue = table->hash(entry->word, strlen(entry->word), table->numberOfBuckets);

	// don't trust hash function to modulo; force modulo one last time
	hashValue %= table->numberOfBuckets;

	// if bucket already occupied, tack it on to the end of the new entry
	if (table->buckets[hashValue]) {
		entry->next = table->buckets[hashValue];
	}

	// occupy the bucket with the new entry
	table->buckets[hashValue] = entry;

	return 1;
}

/*
 * Creates and returns a pointer to a new hash table with the given number of buckets that uses the given
 * hash function.
 */
HashTable_t * createHashTable(int numBuckets, int (*hashFunction)(char *, int, int)) {
	HashTable_t * hashTable = (HashTable_t *)malloc(sizeof(HashTable_t));
	hashTable->numberOfBuckets = numBuckets;
	hashTable->buckets = (Word_t **)malloc(sizeof(Word_t *) * numBuckets);
	hashTable->hash = hashFunction;

	int i;

	for (i = 0; i < numBuckets; ++i) {
		hashTable->buckets[i] = (Word_t *)0;
	}

	return hashTable;
}

/*
 * Finds and returns the entry for the given word, or else returns the null pointer where the search ended.
 */
Word_t * findEntry(HashTable_t * table, char * word, int wordLength) {
	if (!table) {
		printf("Error in findEntry(HashTable_t *, char *, int): Table not defined!\n");
		return (Word_t *)0;
	}

	if (!table->buckets) {
		printf("Error in findEntry(HashTable_t *, char *, int): Table's buckets not defined!\n");
		return (Word_t *)0;
	}

	int hashValue = table->hash(word, wordLength, table->numberOfBuckets);

	hashValue %= table->numberOfBuckets;

	Word_t * chain = table->buckets[hashValue];

	while (chain) {
		if (strcmp(chain->word, word) == 0) {
			break;
		}

		chain = chain->next;
	}

	return chain;
}

/*
 * Determines whether the given word exists in the table.
 */
int entryExists(HashTable_t * table, char * word, int wordLength) {
	if (findEntry(table, word, wordLength)) {
		return 1;
	}

	return 0;
}

/*
 * Returns a hash value calculated by multiplying each character by its position in the word, then taking
 * that value modulo the given modulus.
 */
int cumulativeCharFactorHash(char * word, int wordLength, int modulus) {
	int i;
	int hash = 0;

	for (i = 0; i < wordLength; ++i) {
		hash = (hash + (((int) word[i]) * (i + 1))) % modulus;
	}

	return hash;
}

/*
 * Returns a hash value calculated by adding up the values of each character, then taking that value modulo
 * the given modulus.
 */
int simpleSumHash(char * word, int wordLength, int modulus) {
	int i;
	int hash = 0;

	for (i = 0; i < wordLength; ++i) {
		hash = (hash + ((int) word[i])) % modulus;
	}

	return hash;
}

/*
 * Returns a hash value calculated by multiplying the current hash value by 31, adding the value of the
 * current character, and then taking that value modulo the given modulus.
 */
int constantCharFactorHash(char * word, int wordLength, int modulus) {
	int i;
	int hash = (int) word[0];

	for (i = 1; i < wordLength; ++i) {
		hash = ((hash * 31) + ((int) word[i])) % modulus;
	}

	return hash;
}

void calculateStatistics(HashTable_t * table, double * alpha, double * empty, int * longest, int * average, double * lonely) {
	*alpha = 0.0;
	*empty = 0.0;
	*longest = 0;
	*average = 0;
	*lonely = 0.0;

	int total = 0;
	int occupied = 0;
	Word_t * chain;
	int length;
	int i;

	for (i = 0; i < table->numberOfBuckets; ++i) {
		if (table->buckets[i]) {
			occupied++;

			if (table->buckets[i]->next) {
				chain = table->buckets[i];

				length = 1;

				while (chain->next) {
					chain = chain->next;

					length++;
				}

				if (length > *longest) {
					*longest = length;
				}

				total += length;
			} else {
				total++;
				*lonely += 1.0;
			}
		}
	}

	double div = (double) table->numberOfBuckets;

	*alpha = total / div;
	*average = total / occupied;
	*empty = (table->numberOfBuckets - occupied) / div;
	*lonely /= div;
}
