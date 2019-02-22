#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "symbols.h"
#include "echain.h"
#include "htable.h"

node_t *stack = NULL;

int nSymbols = 0;

void createStack()
{
	// The stack always starts with 'begin' -> 'end'
	pushSymbol("begin", BEGIN);
	pushSymbol("end", END);
}

int lookup(char *symbol)
{
	// Start looking for a symbol at the top of the stack
	
	int pos = nSymbols - 1;
	
	node_t *top = stack;
	
	// Traverse until you find the given symbol or reach the end
	while (top != NULL && strcmp(top->name, symbol) != 0)
	{
		top = traverse(top);
		pos--;
	}
	
	// If the end is reached, the symbol is not in the stack
	if (top == NULL)
	{
		pos = NOT_FOUND;
	}
	
	// Otherwise, return the symbol position
	return pos;
}

void pushSymbol(char *symbol, TYPE type)
{
	// Push the symbol with the given type onto the top of the stack=
	stack = pushNode(stack, createNode(symbol, type));
	
	// Update the count of symbols
	nSymbols++;
}

void identify(char *value)
{
	// Put the given variable into the symbol table
	
	node_t *id = createNode(value, ID);
	
	putNode(symbolTable, id);
}
