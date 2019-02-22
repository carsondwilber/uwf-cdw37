/**
 * Student Name: Carson Wilber
 * File Name: input.h
 * Project 1
 *
 * Character input and tracking.
 */

#ifndef INPUT_H
#define INPUT_H

/**
 * Character number tracked during parsing.
 */
extern int charno;

/**
 * Advances the file to the next character in the
 * stream and returns it.
 *
 * @returns next character in {@code inputFile}
 */
char advance();

#endif // INPUT_H
