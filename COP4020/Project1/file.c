#include <stdio.h>
#include <stdlib.h>

#include "file.h"

FILE *inputFile;

void open(char *fileName)
{
	// Attempt to open the file with the given path or name for reading
	inputFile = fopen(fileName, "r");
	
	// Check that the file opened successfully
	if (NULL == inputFile)
	{
		printf("Error opening file. Exiting.\n");
		exit(-1);
	}
}
