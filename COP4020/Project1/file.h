/**
 * Student Name: Carson Wilber
 * File Name: file.h
 * Project 1
 *
 * Simple field and utility for opening a file.
 */

#ifndef FILE_H
#define FILE_H

/**
 * File pointer for the file to be parsed.
 */
extern FILE *inputFile;

/**
 * Utility to open the file with the given name or path.
 *
 * @param fileName name or path of file to open
 */
void open(char *fileName);

#endif // FILE_H
