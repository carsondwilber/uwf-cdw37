/***************************************************************
  Student Name: Carson Wilber
  File Name: permutation.c
  Project 3

  Defines permutation utilities.
***************************************************************/

#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include "permutation.h"

int *s;
int LEN;

void initS(int N)
{
	if (s)
	{
		free(s);
	}

	LEN = N;

	s = (int *)malloc(sizeof(int) * LEN);

	int i;

	for (i = 0; i < LEN; ++i)
	{
		s[i] = i;
	}
}

void permuteS()
{
	int m, k, p , q, i;

	m = LEN - 2;

	while(s[m] > s[m+1])
	{
		m = m - 1;
	}

	k = LEN-1;

	while(s[m] > s[k])
	{
		k = k - 1;
	}

	swap(s, m, k);
	p = m + 1;
	q = LEN-1;

	while(p < q)
	{
		swap(s, p, q);
		p++;
		q--;
	}
}

void copyS(int *to)
{
	copy(s, to, LEN);
}

void printS()
{
	int i;

	for (i = 0; i < LEN; ++i)
	{
		printf("%d ", s[i]);
	}

	printf("\n");
}
