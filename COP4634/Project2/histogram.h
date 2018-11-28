#ifndef HISTOGRAM_H
#define HISTOGRAM_H

/**
 * Utility function to allocate a historgram integer array with
 * {@code initialCapacity} buckets.
 */
int * allocateHistogram(int initialCapacity);

/**
 * Utility function to reallocate a histogram from current
 * capacity to {@code newCapacity}.
 */
int * extendHistogram(int *histogram, int initialCapacity, int newCapacity);

/**
 * Utility to print histogram data as a list of k, n where
 * n is the frequency of stopping time k.
 */
void printHistogramData(int *histogram, int nBuckets);

/**
 * Utility to properly print a histogram.
 */
void printHistogram(int *histogram, int nBuckets);

#endif // HISTOGRAM_H
