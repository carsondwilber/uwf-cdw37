/*
 * File: filetypes.h
 * Created: 23 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides an interface for reading the file types and MIME types for server files.
 */

#ifndef FILETYPES_H
#define FILETYPES_H

#define FILEEXT_TEXT_HTML	1000
#define FILEEXT_IMAGE_JPEG	1001
#define FILEEXT_IMAGE_PNG	1002

#define FILEEXT_UNKNOWN		1010

#define FILETYPE_TEXT_HTML	"text/html"
#define FILETYPE_IMAGE_JPEG	"image/jpeg"
#define FILETYPE_IMAGE_PNG	"image/png"

/**
 * Description: 
 * 	This function takes a file name and returns a character array of the extension
 *
 * char *fileName:	The file name being passed to the get file extension from
 */
char * getFileExtension(char *fileName);

/**
 * Description: 
 * 	This function takes a file name extension and returns a directive defined value
 *
 * char *fileExt:	This is the file extension being evaluated
 */
int getFileType(char *fileExt);

/**
 * Description: 
 * 	This function takes a file type and returns the MIME (Multipurpose Internet Mail Extensions)
 * type given a file name
 *
 * char *fileName:	The file name being passed to get the MIME type from
 */
char * getMIMEType(int fileType);

#endif // FILETYPES_H
