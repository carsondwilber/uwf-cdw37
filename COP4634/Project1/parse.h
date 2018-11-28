#define MAXARGS 32

/* structure to hold command line arguments */
struct PARAM
{
	char *inputRedirect; /* file name or NULL */
	char *outputRedirect; /* file name or NULL */
	int argumentCount; /* number of tokens in argumentVector */
	char *argumentVector[MAXARGS]; /* array of tokens */
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
