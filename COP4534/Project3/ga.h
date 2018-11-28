/***************************************************************
  Student Name: Carson Wilber
  File Name: ga.h
  Project 3

  Declares parameter and generation structures and genetic
  algorithm utilities.
***************************************************************/

#include "graph.h"

#ifndef GA_H_
#define GA_H_

#define MINIMUM_SOLUTIONS 10
#define MINIMUM_GENERATIONS 2
#define DEFAULT_SWAPS 4
#define DEFAULT_MUTATIONS 0.5

struct Parameters_st
{
	unsigned int nSolutions;
	unsigned int nGenerations;
	unsigned int nMutationSwaps;
	double pMutations;
};

typedef struct Parameters_st Parameters_t;

struct Generation_st
{
	Parameters_t *parameters;
	Solution_t **solutions;
	double *costs;
	unsigned int generation;
	unsigned int lowestCostIndex;
};

typedef struct Generation_st Generation_t;

/**
 * Helper function to allocate a default set of parameters in memory.
 */
Parameters_t * createDefaultParameters();

/**
 * Initializes a Generation using the given Graph and Parameters, then calculates costs for the first set of solutions.
 */
Generation_t * createGeneration(Graph_t *graph, Parameters_t *parameters);

/**
 * Mutates the given solution by making 'm' random swaps.
 */
void mutate(Solution_t *path, int m);

/**
 * Continues the genetic algorithm from the given generation, basing algorithmic decisions on the parameters of the generation.
 *
 * Note: exactly two elites are always kept.
 * Note: twenty-five percent of mutations are mutated from elites.
 */
void nextGeneration(Generation_t *generation);

#endif /* GA_H_ */
