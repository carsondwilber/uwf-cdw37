/*
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcs.h"
#include "io.h"

#define PART_1_FILE_NAME "twoSequences.txt"
#define PART_2_FILE_NAME "multipleSequences.txt"

int main(int argc, char **argv)
{
	printf("===== PART 1 =====\n");

	FILE *file;

	file = fopen(PART_1_FILE_NAME, "rb");

	char * X = readLine(file);
	char * Y = readLine(file);

	fclose(file);

	printf("Sequence X: %s\nSequence Y: %s\n", X, Y);

	int ** c = LCS_Init_C(X, Y);

	LCS_Length(X, Y, c, (char ***)0);

	int m = strlen(X) - 1;
	int n = strlen(Y) - 1;

	int length = c[m][n] + 1;
	char buffer[length--];

	buffer[length--] = '\0';

	while (m != 0 && n != 0)
	{
		if (X[m] == Y[n])
		{
			buffer[length--] = X[m--];
			n--;
		}
		else
		{
			if (c[m-1][n] >= c[m][n-1])
			{
				m--;
			}
			else
			{
				n--;
			}
		}
	}

	printf("LCS: %s\n", buffer);

	LCS_Free(X, Y, c, (char ***)0);

	printf("\n");

	printf("===== PART 2 =====\n");

	file = fopen(PART_2_FILE_NAME, "rb");

	int N;

	sscanf(readLine(file), "%d", &N);

	long firstSequence = ftell(file);
	long last;

	int lengths[N-1];

	char LCSArray[N][N];

	int i, x;
	for (i = 0; i < N - 1; ++i)
	{
		if (i > 0)
		{
			fseek(file, firstSequence, SEEK_SET);
			int z;
			for (z = 0; z < i; ++z)
			{
				fseek(file, lengths[z], SEEK_CUR);
			}
		}

		last = ftell(file);

		free(X);

		X = readLine(file);

		lengths[i] = ftell(file) - last;

		for (x = i + 1; x < N; ++x)
		{
			free(Y);

			Y = readLine(file);

			c = LCS_Init_C(X, Y);

			LCS_Length(X, Y, c, (char ***)0);

			int lenX = strlen(X);
			int lenY = strlen(Y);
			int lenS = lenX > lenY ? lenY : lenX;
			int lenL = lenX > lenY ? lenX : lenY;

			int lcs = c[lenX-1][lenY-1];

			LCSArray[i][x] = 'D';

			if (lenS >= lenL * 0.9)
			{
				if (lcs >= lenS * 0.9)
				{
					LCSArray[i][x] = 'H';
				}
			}

			if (LCSArray[i][x] == 'D' && lenS >= lenL * 0.8)
			{
				if (lcs >= lenS * 0.8)
				{
					LCSArray[i][x] = 'M';
				}
			}

			if (LCSArray[i][x] == 'D' && lenS >= lenL * 0.6)
			{
				if (lcs >= lenS * 0.6)
				{
					LCSArray[i][x] = 'L';
				}
			}

			LCS_Free(X, Y, c, (char ***)0);
		}
	}

	for (i = 0; i < N; ++i)
	{
		fseek(file, firstSequence, SEEK_SET);

		int z;
		for (z = 0; z < i; ++z)
		{
			fseek(file, lengths[z], SEEK_CUR);
		}

		X = readLine(file);

		printf("Sequence %d: %s\n", i + 1, X);
	}

	fclose(file);

	printf("\n");

	for (i = 0; i < N; ++i)
	{
		if (i == 0)
		{
			printf("    ");
			while (++i <= N) printf("%2d  ", i);
			printf("\n");
			i = 0;
		}

		printf("%3d ", i+1);

		for (x = 0; x < N; ++x)
		{
			printf(" %c  ", x < i + 1 ? '-' : LCSArray[i][x]);
		}

		printf("\n");
	}

	return 0;
}
