/***************************************************************
  Student Name: Carson Wilber
  File Name: util.c
  Project 3

  Defines useful integer array utilities, swap and copy.
***************************************************************/

#include "util.h"

/**
 * Helper function to swap two points in an integer array.
 */
void swap(int *s, int i, int j)
{
	int tmp = s[i];
	s[i] = s[j];
	s[j] = tmp;
}

/**
 * Helper function to copy one array of N points to another.
 */
void copy(int *from, int *to, int N)
{
	int i;

	for (i = 0; i < N; ++i)
	{
		to[i] = from[i];
	}
}
