/***************************************************************
  Student Name: Carson Wilber
  File Name: graph.h
  Project 3

  Declares graph and solution structures and utilities.
***************************************************************/

#ifndef GRAPH_H_
#define GRAPH_H_

struct Graph_st
{
	int N;
	double **weights;
};

typedef struct Graph_st Graph_t;

struct Solution_st
{
	Graph_t *graph;
	int *points;
};

typedef struct Solution_st Solution_t;

/**
 * Helper function to allocate a graph in memory.
 */
Graph_t * createEmptyGraph();

/**
 * Helper function to allocate a graph solution in memory.
 */
Solution_t * createEmptySolution(Graph_t *graph);

/**
 * Load a weight graph from a given file.
 */
Graph_t * loadGraph(char *fileName, int N);

/**
 * Using the weight graph provided, calculate the cost of the given solution.
 */
double calculateCost(Solution_t *solution, Graph_t *graph);

/**
 * Use a brute-force approach to produce the optimal solution for the given graph.
 */
Solution_t * findOptimalSolution(Graph_t *graph);

#endif /* GRAPH_H_ */
