/*
 * File: fileio.h
 * Created: 24 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides an interface for allocating a buffer to read in the contents of a file.
 */

#ifndef FILEIO_H
#define FILEIO_H

/**
 * Description: 
 * 	This function takes a file and reads it into a given buffer and returns either a -1 for an error or
 * the length of the file data
 *
 * char *filePath:	The file path to the file the caller wishes to read into a buffer
 * char **fileData:	The buffer the file is being read into
 */
int readFileIntoBuffer(char *filePath, char **fileData);

#endif
