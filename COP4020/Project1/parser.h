/**
 * Student Name: Carson Wilber
 * File Name: parser.h
 * Project 1
 *
 * Functions to start and perform the parsing process.
 */

#include "types.h"
#include "echain.h"

#ifndef PARSER_H
#define PARSER_H

/**
 * If {@code lookahead} is equal to the given TYPE or character
 * {@code i}, sets lookahead to the next token. Otherwise, prints
 * an error message indicating the mismatch and exits the program.
 *
 * @param i TYPE or character expected as the next token
 */
void match(int i);

/**
 * Matches a factor, i.e. one or more ID or NUM, which altogether
 * may be an expression in parentheses.
 */
void factor();

/**
 * Matches a term, i.e. a combination of ID and NUM separated by
 * the multiplication or division symbols.
 */
void term();

/**
 * Matches an expression, i.e. a combination of ID and NUM separated by
 * the addition or subtraction symbols.
 */
void expression();

/**
 * Matches an assignment expression, i.e. an identifier, an equals sign,
 * and any expression, which must be composed of raw number values or
 * of identifiers previously assigned to other expressions or raw number
 * values.
 */
void statement();

/**
 * Parses the file opened by {@code lexparser()}.
 */
void parse();

#endif // PARSER_H
