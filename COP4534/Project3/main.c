/***************************************************************
  Student Name: Carson Wilber
  File Name: main.c
  Project 3

  Attempts brute-force and genetic algorithm solutions to the
  Traveling Salesman Problem.
***************************************************************/

#include "graph.h"
#include "ga.h"
#include "timer.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int N = 0;
	Parameters_t *parameters = createDefaultParameters();

	printf("Enter number of points to use (minimum 3, maximum 20): ");
	scanf("%i", &N);

	while (N < 3 || N > 20)
	{
		printf("Must be greater than 3 and less than 20.\n");
		printf("Enter number of points to use (minimum 3, maximum 20): ");
		scanf("%i", &N);
	}

	printf("Enter number of solutions in each generation (minimum 10): ");
	scanf("%u", &parameters->nSolutions);

	while (parameters->nSolutions < MINIMUM_SOLUTIONS)
	{
		printf("Enter number of solutions in each generation (minimum 10): ");
		scanf("%u", &parameters->nSolutions);
	}

	printf("Enter number of generations to run (minimum 2): ");
	scanf("%u", &parameters->nGenerations);

	while (parameters->nGenerations < MINIMUM_GENERATIONS)
	{
		printf("Enter maximum number of generations to run (minimum 2): ");
		scanf("%u", &parameters->nGenerations);
	}

	printf("Enter percentage of mutations per generation (minimum 0.0, maximum 80.0): ");
	scanf("%lf", &parameters->pMutations);

	while (parameters->pMutations < 0.0 || parameters->pMutations >= 80.0)
	{
		printf("Enter percentage of mutations per generation (minimum 0.0, maximum 80.0): ");
		scanf("%lf", &parameters->pMutations);
	}

	parameters->pMutations /= 100.0;

	Graph_t *graph = loadGraph("cityWeights.txt", N);

	if (!graph)
	{
		printf("Shutting down...\n");
		return -1;
	}

	startTimer();

	Solution_t *solution = findOptimalSolution(graph);

	time_t bruteForceTime = endTimer();

	double optimalCost = calculateCost(solution, graph);

	printf("It took the brute force algorithm %d seconds to find the optimal solution with a cost of %.2lf.\n", bruteForceTime, optimalCost);

	printf("The optimal solution is: ");

	int i;

	for (i = 0; i < N - 1; ++i)
	{
		printf("%d ", solution->points[i]);
	}

	printf("%d\n", solution->points[N - 1]);

	startTimer();

	Generation_t *generation = createGeneration(graph, parameters);

	while (generation->generation < parameters->nGenerations && generation->costs[generation->lowestCostIndex] > 1.10 * optimalCost)
	{
		nextGeneration(generation);
	}

	time_t generativeAlgorithmTime = endTimer();

	printf("It took the genetic algorithm %d seconds to find a solution with cost %.2lf, or %.2lf%% of the optimal cost.\n", generativeAlgorithmTime, generation->costs[generation->lowestCostIndex], 100.0 * generation->costs[generation->lowestCostIndex] / optimalCost);

	printf("The genetic algorithm's solution is: ");

	for (i = 0; i < N - 1; ++i)
	{
		printf("%d ", generation->solutions[generation->lowestCostIndex]->points[i]);
	}

	printf("%d\n", generation->solutions[generation->lowestCostIndex]->points[i]);

	return 0;
}
