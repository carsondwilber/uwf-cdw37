#include <stdbool.h>

#ifndef PARSE_H
#define PARSE_H

#define MAXARGS 32

/* structure to hold command line arguments */
struct PARAM
{
	char *redirects[3]; /* redirect file names or NULL */
	bool append[2]; /* whether or not stdout/stderr are appending */
	int flagCount; /* number of tokens in flagVector */
	int argumentCount; /* number of tokens in argumentVector */
	char *flagVector[MAXARGS]; /* array of flags */
	char *argumentVector[MAXARGS]; /* array of other arguments */
};

/* a typedef so we don't have to use "struct PARAM" all the time */
typedef struct PARAM Param_t;

/**
 * A utility for tokenizing input commands, interpreting and
 * storing input and output redirects, and counting the number
 * of arguments submitted.
 *
 * @param buffer Input buffer holding command line submission
 * @return PARAM struct holding parsed arguments
 */
Param_t * parseArguments(char *buffer);

/**
 * A utility for checking a string array for the given flag.
 */
bool checkFlag(char **vector, int size, char *flag);

#endif // PARSE_H
