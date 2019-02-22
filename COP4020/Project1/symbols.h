/**
 * Student Name: Carson Wilber
 * File Name: symbols.h
 * Project 1
 *
 * Fields and functions for creating and manipulating the
 * symbol table.
 */

#include <stdlib.h>

#include "htable.h"
#include "echain.h"

#ifndef SYMBOLS_H
#define SYMBOLS_H

/**
 * Current symbol stack.
 */
extern node_t *stack;

/**
 * Number of symbols on the stack.
 */
extern int nSymbols;

/**
 * Sets the {@code stack} pointer to a new stack.
 */
void createStack();

/**
 * Looks up the given symbol name in the symbol table.
 *
 * @param symbol name of the symbol to look up
 * @returns int index of symbol's place in stack if found,
 * 	or {code NOT_FOUND} otherwise
 */
int lookup(char *symbol);

/**
 * Pushes the given {@code symbol} with the given {@code type}
 * onto the stack.
 *
 * @param symbol name of the new symbol
 * @param type TYPE of the new symbol
 */
void pushSymbol(char *symbol, TYPE type);

/**
 * Pushes the given {@code value} as an {@code ID} onto the
 * stack.
 *
 * @param value name of the identifier
 */
void identify(char *value);

#endif // SYMBOLS_H
