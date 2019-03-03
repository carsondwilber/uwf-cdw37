/*
 * File: fileio.c
 * Created: 24 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides implementation for allocating a buffer to read in the contents of a file.
 */

#include <stdio.h>
#include <stdlib.h>

#include "fileio.h"
#include "logging.h"

int readFileIntoBuffer(char *filePath, char **fileData)
{
	FILE *fp = fopen(filePath, "r");
	
	if (fp != NULL)
	{
		/* Go to the end of the file. */
		if (fseek(fp, 0L, SEEK_END) == 0)
		{
			/* Get the size of the file. */
			long bufsize = ftell(fp);

			if (bufsize == -1)
			{
				log_msg(ERROR, "Error reading file!");
				
				fclose(fp);
				
				return -1;
			}

			/* Allocate our buffer to that size. */
			*fileData = malloc(sizeof(char) * (bufsize + 1));

			/* Go back to the start of the file. */
			if (fseek(fp, 0L, SEEK_SET) != 0)
			{
				log_msg(ERROR, "Error reading file!");
				
				free(*fileData);
				
				fclose(fp);
				
				return -1;
			}

			/* Read the entire file into memory. */
			size_t newLen = fread(*fileData, sizeof(char), bufsize, fp);
			
			if (ferror(fp) != 0)
			{
				log_msg(ERROR, "Error reading file!");
				
				free(*fileData);
				
				fclose(fp);
				
				return -1;
			}
			else
			{
				(*fileData)[newLen++] = '\0'; /* Just to be safe. */
			}
			
			return newLen;
		}
		
		log_msg(ERROR, "Error reading file!");
		
		fclose(fp);
		
		return -1;
	}
	
	log_msg(ERROR, "Error opening file!");
	
	fclose(fp);
	
	return -1;
}
