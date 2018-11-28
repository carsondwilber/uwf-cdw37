/*
 *
 */

#ifndef LCS_H_
#define LCS_H_

int ** LCS_Init_C(char * X, char * Y);

char *** LCS_Init_B(char * X, char * Y);

void LCS_Free(char * X, char * Y, int **c, char ***b);

void LCS_Length(char * X, char * Y, int **c, char ***b);

#endif /* LCS_H_ */
