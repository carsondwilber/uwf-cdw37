/**
 * Student Name: Carson Wilber
 * File Name: lexical.h
 * Project 1
 *
 * Fields and functions to start and perform the lexical
 * analysis.
 */

#include "types.h"
#include "echain.h"

#ifndef LEXICAL_H
#define LEXICAL_H

/**
 * Indicator for next token, or the next character.
 */
extern int lookahead;

/**
 * Full token string for identifiers.
 */
extern char *value;

/**
 * Full token string for numbers.
 */
extern char *numLexeme;

/**
 * Lexically analyzes the file opened for parsing and
 * returns either an indicator of the type of the next
 * token, or the next character in the file for special
 * characters (parentheses, operators, etc.)
 *
 * @returns TYPE or character value of next token
 */
int lexan();

#endif // LEXICAL_H
