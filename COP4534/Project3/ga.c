/***************************************************************
  Student Name: Carson Wilber
  File Name: ga.c
  Project 3

  Defines genetic algorithm utilities.
***************************************************************/

#include "graph.h"
#include "ga.h"
#include "permutation.h"
#include "util.h"
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

Parameters_t * createDefaultParameters()
{
	Parameters_t *parameters = (Parameters_t *)malloc(sizeof(Parameters_t));

	parameters->nSolutions = MINIMUM_SOLUTIONS;

	parameters->nGenerations = MINIMUM_GENERATIONS;

	parameters->nMutationSwaps = DEFAULT_SWAPS;

	parameters->pMutations = DEFAULT_MUTATIONS;

	return parameters;
}

Generation_t * createGeneration(Graph_t *graph, Parameters_t *parameters)
{
	Generation_t *generation = (Generation_t *)malloc(sizeof(Generation_t));

	generation->parameters = parameters;

	generation->solutions = (Solution_t **)malloc(sizeof(Solution_t *) * parameters->nSolutions);

	generation->costs = (double *)malloc(sizeof(double) * parameters->nSolutions);

	generation->generation = 0;

	generation->lowestCostIndex = 0;

	initS(graph->N);

	int i;

	for (i = 0; i < parameters->nSolutions; ++i)
	{
		generation->solutions[i] = createEmptySolution(graph);

		copyS(generation->solutions[i]->points);

		generation->costs[i] = calculateCost(generation->solutions[i], graph);

		if (generation->costs[i] < generation->costs[generation->lowestCostIndex])
		{
			generation->lowestCostIndex = i;
		}

		permuteS();
	}

	return generation;
}

void mutate(Solution_t *path, int m)
{
	int i;
	int x, y;

	for (i = 0; i < m; ++i)
	{
		x = rand() % path->graph->N;
		y = rand() % path->graph->N;

		swap(path->points, x, y);
	}
}

void nextGeneration(Generation_t *generation)
{
	int first = 0, second = 0;
	int lowest = 0;
	int i;

	for (i = 1; i < generation->parameters->nSolutions; ++i)
	{
		if (generation->costs[i] < generation->costs[lowest])
		{
			lowest = i;
		}
	}

	first = lowest;
	lowest = (first == 0 ? 1 : 0);

	for (i = (lowest == 0 ? 1 : 0); i < generation->parameters->nSolutions; ++i)
	{
		if (i == first) i++;

		if (i == generation->parameters->nSolutions) break;

		if (generation->costs[i] < generation->costs[lowest])
		{
			lowest = i;
		}
	}

	second = lowest;

	Solution_t *eliteOne = generation->solutions[first];
	Solution_t *eliteTwo = generation->solutions[second];

	if (first != 0)
	{
		Solution_t *tmp = generation->solutions[0];
		generation->solutions[0] = eliteOne;
		generation->solutions[first] = tmp;

		if (second == 0)
		{
			second = first;
		}

		first = 0;
	}

	if (second != 1)
	{
		Solution_t *tmp = generation->solutions[1];
		generation->solutions[1] = eliteTwo;
		generation->solutions[second] = tmp;

		second = 1;
	}

	// Copy and mutate elites
	for (i = 0; i < (int)(((double)generation->parameters->nSolutions) * generation->parameters->pMutations * 0.25); ++i)
	{
		copy(generation->solutions[i % 2]->points, generation->solutions[i + 2]->points, generation->solutions[i % 2]->graph->N);
		mutate(generation->solutions[i + 2], generation->parameters->nMutationSwaps);
	}

	// Mutate remaining mutation percentage of existing permutations
	for (; i < (int)(((double)generation->parameters->nSolutions) * generation->parameters->pMutations); ++i)
	{
		mutate(generation->solutions[i + 2], generation->parameters->nMutationSwaps);
	}

	// Insert new permutations
	for (; i < generation->parameters->nSolutions - 2; ++i)
	{
		copyS(generation->solutions[i + 2]->points);
		permuteS();
	}

	generation->lowestCostIndex = 0;

	for (i = 0; i < generation->parameters->nSolutions; ++i)
	{
		generation->costs[i] = calculateCost(generation->solutions[i], generation->solutions[i]->graph);

		if (generation->costs[i] < generation->costs[generation->lowestCostIndex])
		{
			generation->lowestCostIndex = i;
		}
	}

	generation->generation++;
}
