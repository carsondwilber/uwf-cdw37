/*
 * File: filetypes.c
 * Created: 23 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides implementation for reading the file types and MIME types for server files.
 */

#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "logging.h"
#include "filetypes.h"

char * getFileExtension(char *fileName)
{
	if (fileName == NULL)
	{
		log_msg(ERROR, "Cannot read file extension of null file!");
		
		return NULL;
	}
	
	char *ptr, *last;
	
	int len = strlen(fileName);
	
	char *fileNameCopy = (char *)malloc(sizeof(char) * len);
	
	strncpy(fileNameCopy, fileName, len);
	
	fileNameCopy[len] = '\0';
	
	ptr = strtok(fileNameCopy, ".");
	
	if (ptr == NULL)
	{
		log_msg(ERROR, "Cannot read file extension of an unnamed file!");
		
		return NULL;
	}
	
	ptr = strtok(NULL, ".");
	
	if (ptr == NULL)
	{
		log_msg(ERROR, "Cannot read file extension of a file without an extension!");
		
		return NULL;
	}
	
	do
	{
		last = ptr;
		
		ptr = strtok(NULL, ".");
	} while (ptr != NULL);
	
	return last;
}

int getFileType(char *fileExt)
{
	if (strcicmp(fileExt, "html") == 0)
	{
		return FILEEXT_TEXT_HTML;
	}
	else if (strcicmp(fileExt, "jpg") == 0 || strcicmp(fileExt, "jpeg") == 0)
	{
		return FILEEXT_IMAGE_JPEG;
	}
	else if (strcicmp(fileExt, "png") == 0)
	{
		return FILEEXT_IMAGE_PNG;
	}
	
	return FILEEXT_UNKNOWN;
}

char * getMIMEType(int fileType)
{
	char *ret = NULL;
	
	switch (fileType)
	{
	case FILEEXT_TEXT_HTML:
	{
		ret = FILETYPE_TEXT_HTML;
		break;
	}
	
	case FILEEXT_IMAGE_JPEG:
	{
		ret = FILETYPE_IMAGE_JPEG;
		break;
	}
	
	case FILEEXT_IMAGE_PNG:
	{
		ret = FILETYPE_IMAGE_PNG;
		break;
	}
	}
	
	return ret;
}
