#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parse.h"
#include "histogram.h"
#include "collatz.h"

// Gloal mutex for locking
pthread_mutex_t mutex;

// Global flag for whether or not locking is enabled (enabled by default)
bool lock = true;

// Global counter and bound for computing collatz sequences
int COUNTER = 2, N;

// Global number of buckets (could be used for dynamically extending size
// of histogram as needed - see lines 45-49
int nBuckets;

// Histogram of collatz values
int *histogram;

// Structs for holding realtime clock values
struct timespec start, end;

// Struct containing parsed parameters
Param_t *params;

/**
 * Thread function to be used by each pthread. Calculates collatz sequences
 * for the next {@code COUNTER} value, with or without threads as determined
 * by {@code lock}.
 */
void * threadCollatz(void *arg)
{
	// Lock, if enabled, for while loop condition
	if (lock) pthread_mutex_lock(&mutex);
	
	// Continue calculating collatz sequences until N is reached
	while (COUNTER <= N)
	{
		// Set nCollatz to current value to be processed and increment the counter
		int nCollatz = COUNTER++;
		
		// Unlock, if enabled, for collatz computation
		if (lock) pthread_mutex_unlock(&mutex);
		
		// Set kCollatz to stopping time of nCollatz
		int kCollatz = collatz(nCollatz);
		
		// Lock, if enabled, for histogram dynamic extension and incrementing
		if (lock) pthread_mutex_lock(&mutex);
		
		// Extend histogram, if needed
		while (kCollatz >= nBuckets)
		{
			histogram = extendHistogram(histogram, nBuckets, nBuckets * 2);
			nBuckets *= 2;
		}

		// Increment collatz stopping time frequency
		histogram[kCollatz]++;
	}
	
	// Unlock, if enabled
	if (lock) pthread_mutex_unlock(&mutex);
	
	pthread_exit(0);	
}

/**
 * Utility function for joining argv into a single string for parsing by
 * {@code parseArguments()}.
 */
char * join(int argc, char **argv)
{
	int i, len;
	
	for (i = 0, len = argc; i < argc; ++i) len += strlen(argv[i]);
	
	char *cmd = (char *)malloc(sizeof(char) * len);
	
	for (i = 0, len = 0; i < argc; ++i, len += strlen(argv[i - 1]) + 1)
	{
		strcpy(cmd + len, argv[i]);
		cmd[len + strlen(argv[i])] = ' ';
	}
	
	cmd[len - 1] = '\0';
	
	return cmd;
}

/**
 * Utility function for redirecting input/output streams using {@code params}.
 */
void redirect()
{
	// Redirect stdin, if enabled
	if (params->redirects[0] != NULL)
	{
		freopen(params->redirects[0], "r", stdin);
	}
	
	// Redirect stdout, if enabled
	if (params->redirects[1] != NULL)
	{
		freopen(params->redirects[1], params->append[0] ? "a+" : "w+", stdout);
	}
	
	// Redirect stderr, if enabled
	if (params->redirects[2] != NULL)
	{
		freopen(params->redirects[2], params->append[1] ? "a+" : "w+", stderr);
	}
}

int main(int argc, char **argv)
{
	// Require mt-collatz, N, T at least
	if (argc >= 3)
	{
		// Start the computation clock
		clock_gettime(CLOCK_REALTIME, &start);
		
		int nThreads;
		
		// Parse arguments to get N, T, and -nolock flag (if present)
		char *cmd = join(argc - 1, argv + 1);
		params = parseArguments(cmd);
		
		// Set whether or not locking is enabled based on presence of -nolock flag
		lock = !checkFlag(params->flagVector, params->flagCount, "nolock");
		
		// Require N, T at least
		if (params->argumentCount >= 2)
		{
			// Take N to be the first argument
			N = (int) strtol(params->argumentVector[0], NULL, 10);
			
			// Take T to be the second argument
			nThreads = (int) strtol(params->argumentVector[1], NULL, 10);
			
			// Require N > 0, T > 0
			if (N > 0 && nThreads > 0)
			{
				// Allocate initial histogram as size nBuckets
				histogram = allocateHistogram(nBuckets = 50);
				
				// Initialize threading
				pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);
				int thread;
				
				// Initialize mutex
				pthread_mutex_init(&mutex, NULL);
				
				// Start all threads
				for (thread = 0; thread < nThreads; ++thread)
				{
					pthread_create(&threads[thread], NULL, threadCollatz, NULL);
				}
				
				// Wait for all threads to complete
				for (thread = 0; thread < nThreads; ++thread)
				{
					pthread_join(threads[thread], NULL);
				}
				
				// Destroy mutex
				pthread_mutex_destroy(&mutex);
				
				// Print histogram data to stdout
				printHistogramData(histogram, nBuckets);
				
				// Stop the computation clock
				clock_gettime(CLOCK_REALTIME, &end);
				
				// Print N, T, time to stderr
				fprintf(stderr, "%d,%d,%f\n", N, nThreads, ((double) (end.tv_sec - start.tv_sec)) + ((double) (end.tv_nsec - start.tv_nsec)) / 1e9);
			}
			else
			{
				fprintf(stderr, "N and T must be greater than 0.\n");
			}
		}
		else
		{
			fprintf(stderr, "Usage: mt-collatz <N> <T> [-nolock]\n");
		}
	}
	else
	{
		fprintf(stderr, "Usage: mt-collatz <N> <T> [-nolock]\n");
	}
	
	return 0;
}
