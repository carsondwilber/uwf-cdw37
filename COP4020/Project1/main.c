#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "symbols.h"
#include "lexical.h"
#include "parser.h"
#include "echain.h"
#include "htable.h"
#include "input.h"
#include "lexparser.h"

int main(int argc, char *argv[])
{
	// There must be one argument provided that is the name or path of the file to parse.
	if (argc < 2)
	{
		printf("Please provide a command line argument that is the name or path of the file to be parsed.\n");
		exit(-1);
	}
	
	// Create the stack and symbol table
	
	createStack();
	
	createTable();
	
	// Start the lexparse process

	lexparse(argv[1]);
	
	// Attempt to parse the file

	parse();
	
	// Successfully parsed!

	// Print all symbols used in the program.

	node_t *currentSymbol = stack;
	
	printf("Symbols used:\n");
	
	int i;
	for (i = nSymbols; i > 2; --i)
	{
		printf("%s\n", currentSymbol->name);
		currentSymbol = currentSymbol->next;
	}
	
	return 0;
}
