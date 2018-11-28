/*
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcs.h"

int ** LCS_Init_C(char * X, char * Y)
{
	int m = strlen(X);
	int n = strlen(Y);

	int ** c = (int **)malloc(sizeof(int *) * m);

	int i;
	for (i = 0; i < m; ++i)
	{
		c[i] = (int *)malloc(sizeof(int) * n);
	}

	return c;
}

char *** LCS_Init_B(char * X, char * Y)
{
	int m = strlen(X);
	int n = strlen(Y);

	char *** b = (char ***)malloc(sizeof(char **) * m);

	int i;
	for (i = 0; i < m; ++i)
	{
		b[i] = (char **)malloc(sizeof(char *) * n);
	}

	return b;
}

void LCS_Free(char * X, char * Y, int **c, char ***b)
{
	int m = strlen(X);

	int i;

	for (i = 0; i < m; ++i)
	{
		free(c[i]);
		if (b) free(b[i]);
	}

	free(c);
	if (b) free(b);
}

void LCS_Length(char * X, char * Y, int **c, char ***b)
{
	int m = strlen(X);
	int n = strlen(Y);

	int i, j;
	for (i = 1; i < m; ++i)
	{
		c[i][0] = 0;
	}

	for (j = 0; j < n; ++j)
	{
		c[0][j] = 0;
	}

	for (i = 1; i < m; ++i)
	{
		for (j = 1; j < n; ++j)
		{
			if (X[i] == Y[j])
			{
				c[i][j] = c[i-1][j-1] + 1;
				if (b) b[i][j] = "diagonal";
			}
			else
			{
				if (c[i-1][j] >= c[i][j-1])
				{
					c[i][j] = c[i-1][j];
					if (b) b[i][j] = "up";
				}
				else
				{
					c[i][j] = c[i][j-1];
					if (b) b[i][j] = "left";
				}
			}
		}
	}
}
