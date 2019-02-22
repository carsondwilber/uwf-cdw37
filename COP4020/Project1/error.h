/**
 * Student Name: Carson Wilber
 * File Name: error.h
 * Project 1
 *
 * Utilities and fields for error management.
 */

#ifndef ERROR_H
#define ERROR_H

/**
 * Line number tracked during parsing.
 */
extern int lineno;

/**
 * Utility to print an error message with line and character
 * tracking.
 *
 * @param msg error message to display
 */
void error(char *msg);

#endif // ERROR_H
