/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "io.h"

char * readLine(FILE *file)
{
	int length = 0;

	int bufferSize = 128;

	char * buffer = (char *)malloc(sizeof(char) * bufferSize);

	char c = getc(file);

	while (c != '\r' && c != '\n' && c != '\0' && c != EOF)
	{
		if (length == bufferSize)
		{
			bufferSize *= 2;
			buffer = realloc(buffer, bufferSize);
		}

		buffer[length++] = c;

		c = getc(file);
	}

	// Dispose of newline if DOS-style line endings
	if (c == '\r')
	{
		c = getc(file);
	}

	buffer[length] = '\0';

	return buffer;
}
