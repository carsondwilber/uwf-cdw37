/***************************************************************
  Student Name: Carson Wilber
  File Name: util.h
  Project 3

  Declares useful integer array utilities, swap and copy.
***************************************************************/

#ifndef UTIL_H_
#define UTIL_H_

/**
 * Swap the integers at the given positions i, j in the given integer array.
 */
void swap(int *a, int i, int j);

/**
 * Copy the integer array at 'from' to the integer array at 'to' up to 'N' positions, assuming both are of equal or longer length.
 */
void copy(int *from, int *to, int N);

#endif /* UTIL_H_ */
