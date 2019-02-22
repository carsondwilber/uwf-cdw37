#include <stdio.h>

#include "lexparser.h"
#include "file.h"
#include "lexical.h"

void lexparse(char *fileName)
{
	// Open the file
	open(fileName);
	
	// Begin lexical analysis by reading the first token
	lookahead = lexan();
}
