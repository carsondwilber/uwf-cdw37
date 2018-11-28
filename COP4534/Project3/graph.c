/***************************************************************
  Student Name: Carson Wilber
  File Name: graph.c
  Project 3

  Defines graph and solution utilities.
***************************************************************/

#include "graph.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "permutation.h"

Graph_t * createEmptyGraph()
{
	Graph_t *newGraph = (Graph_t *)malloc(sizeof(Graph_t));

	newGraph->N = 0;

	newGraph->weights = (double **)0;

	return newGraph;
}

Solution_t * createEmptySolution(Graph_t *graph)
{
	Solution_t *newSolution = (Solution_t *)malloc(sizeof(Solution_t));

	newSolution->graph = graph;

	newSolution->points = (int *)malloc(sizeof(int) * graph->N);

	int i;

	for (i = 0; i < graph->N; ++i)
	{
		newSolution->points[i] = i;
	}

	return newSolution;
}

Graph_t * loadGraph(char *fileName, int N)
{
	if (N <= 1)
	{
		printf("ERROR: loadGraph(): N must be greater than 1.\n");
		return (Graph_t *)0;
	}

	FILE *file = fopen(fileName, "r");

	if (file == NULL)
	{
		printf("ERROR: loadGraph(): File %s does not exist!\n", fileName);
		return (Graph_t *)0;
	}

	// Initialize the new graph
	Graph_t *graph = createEmptyGraph();
	graph->N = N;
	graph->weights = (double **)malloc(sizeof(double *) * N);

	// Helper variables
	int i = 0;
	int j = 0;
	int r = 0;
	int x;
	double ignore;

	while (i < N)
	{
		graph->weights[i] = (double *)malloc(sizeof(double) * N);

		j = 0;

		while (j < N)
		{
			if (j == i)
			{
				graph->weights[i][j] = 0.0;
			}
			else
			{
				fscanf(file, "%lf\n", &graph->weights[i][j]);
				r++;
			}

			j++;

			if (feof(file))
			{
				break;
			}
		}

		i++;
	}

	fclose(file);

	// If we haven't read in N * (N - 1) weights, the file is too short
	if (r != N * (N - 1))
	{
		printf("ERROR: loadGraph(): Weights file does not contain enough weights!\n");
		free(graph->weights);
		free(graph);
		return (Graph_t *)0;
	}

	return graph;
}

double calculateCost(Solution_t *solution, Graph_t *graph)
{
	double cost = 0.0;
	int i;

	for (i = 0; i < graph->N - 1; ++i)
	{
		cost += graph->weights[solution->points[i]][solution->points[i + 1]];
	}

	return cost;
}

/**
 * Helper function to efficiently calculate the factorial of N.
 */
int factorial(int N)
{
	unsigned int ret = 1;

	while (N > 1) {
		ret *= N--;
	}

	return ret;
}

Solution_t * findOptimalSolution(Graph_t *graph)
{
	Solution_t *solution = createEmptySolution(graph);

	double lowestCost = DBL_MAX;
	int lowestSolution[graph->N];

	int f = factorial(graph->N);
	int m, k, p , q, i;

	initS(graph->N);

	for (i = 1; i < f; i++)
	{
		printS();

		copyS(solution->points);

		double cost = calculateCost(solution, graph);

		if (cost < lowestCost)
		{
			lowestCost = cost;
			copyS(lowestSolution);
		}

		permuteS();
	}

	copy(lowestSolution, solution->points, graph->N);

	return solution;
}
