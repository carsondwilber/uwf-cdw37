/*
 *
 */

#include <stdio.h>

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

struct ChipConfiguration_st
{
	int numBatches;
	int numBatchItems;
	int percentBadBatches;
	int percentBadItems;
	int sampleSize;
};

typedef struct ChipConfiguration_st ChipConfiguration_t;

struct ReadingConfiguration_st
{
	int totalReadings;
	int ranges;
	int * mins;
	int * maxes;
	int * values;
};

typedef struct ReadingConfiguration_st ReadingConfiguration_t;

ChipConfiguration_t * createEmptyChipConfiguration();

ReadingConfiguration_t * createEmptyReadingConfiguration();

ChipConfiguration_t * loadChipConfiguration(char *fileName);

ReadingConfiguration_t * loadReadingConfiguration(char *fileName);

void printChipConfiguration(ChipConfiguration_t *config);

void printReadingConfiguration(ReadingConfiguration_t *config);

void freeChipConfiguration(ChipConfiguration_t *config);

void freeReadingConfiguration(ReadingConfiguration_t *config);

#endif /* CONFIGURATION_H_ */
