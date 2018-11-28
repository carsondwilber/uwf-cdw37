/***************************************************************
  Student Name: Carson Wilber
  File Name: permutation.h
  Project 3

  Declares permutation utilities.
***************************************************************/

#ifndef PERMUTATION_H_
#define PERMUTATION_H_

/**
 * Initialize S as the first permutation of length N.
 */
void initS(int N);

/**
 * Permute S once.
 */
void permuteS();

/**
 * Copy S to the given integer array, assuming it is of the proper length.
 */
void copyS(int *to);

/**
 * Print S as a space-separated string of integers.
 */
void printS();

#endif /* PERMUTATION_H_ */
