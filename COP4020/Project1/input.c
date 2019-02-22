#include <stdlib.h>
#include <stdio.h>

#include "input.h"
#include "file.h"

int charno = 1;

char advance()
{
	// Return EOF if done parsing
	if (feof(inputFile))
	{
		return EOF;
	}
	
	// Otherwise, increment the char number...
	charno++;
	
	// ...and return the next character.
	return (char)fgetc(inputFile); 
}
