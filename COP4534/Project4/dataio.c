/*
 *
 */

#include "dataio.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

void createDirectory(char *path)
{
	int e;
	struct stat s;

	e = stat(path, &s);

	if (e == -1)
	{
		e = mkdir(path, S_IWUSR | S_IRUSR);

		if (e != 0)
		{
			printf("Could not make directory '%s'.\n", path);
		}
	}
}
