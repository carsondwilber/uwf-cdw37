#include <stdio.h>
#include <stdlib.h>
#include "histogram.h"

int * allocateHistogram(int initialCapacity)
{
	int * histogram = (int *)malloc(sizeof(int) * initialCapacity);
	
	int i;
	for (i = 0; i < initialCapacity; ++i)
	{
		histogram[i] = 0;
	}
	
	return histogram;
}

int * extendHistogram(int *histogram, int initialCapacity, int newCapacity)
{
	histogram = (int *)realloc(histogram, sizeof(int) * newCapacity);
	
	int i;
	for (i = initialCapacity; i < newCapacity; ++i)
	{
		histogram[i] = 0;
	}
	
	return histogram;
}

void printHistogramData(int *histogram, int nBuckets)
{
	int i;
	
	for (i = 1; i < 1000; ++i)
	{
		printf("%d,", i);
		
		if (nBuckets > i)
		{
			printf("%d\n", histogram[i]);
		}
		else
		{
			printf("0\n");
		}
	}
}

void printHistogram(int *histogram, int nBuckets)
{
	int i;
	int max = -1;
	int nActual = 0;
	
	for (i = 0; i < nBuckets; ++i)
	{
		if (histogram[i] > 0)
		{
			nActual = i + 1;
			
			if (histogram[i] > max)
			{
				max = histogram[i];
			}
		}
	}
	
	while (max > 0)
	{
		fprintf(stderr, "\n%-3d", max);
		
		for (i = 0; i < nActual; ++i)
		{
			if (histogram[i] >= max)
			{
				fprintf(stderr, "%3s", " * ");
			}
			else
			{
				fprintf(stderr, "%3s", "");
			}
		}
		
		max--;
	}
	
	fprintf(stderr, "\n   ");
	
	for (i = 0; i < nActual; ++i)
	{
		fprintf(stderr, "%3d", i);
	}
	
	fprintf(stderr, "\n");
}
