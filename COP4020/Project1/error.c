#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "input.h"

int lineno = 1;

void error(char *msg)
{
	printf("[Line %d | Char %d]: %s\n", lineno, charno, msg);
	exit(-1);
}
