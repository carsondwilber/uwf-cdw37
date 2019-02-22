/**
 * Student Name: Carson Wilber
 * File Name: lexparser.h
 * Project 1
 *
 * Provides a single wrapper around opening the given file
 * and beginning the lexical analysis process by calling
 * {@code lexan()}.
 */

#ifndef LEXPARSER_H
#define LEXPARSER_H

/**
 * Opens the file with the given name or path, then
 * uses {@code lexan()} to set lookahead to the first
 * token in the file.
 *
 * @param fileName name of the file to open
 */
void lexparse(char *fileName);

#endif // LEXPARSER_H
