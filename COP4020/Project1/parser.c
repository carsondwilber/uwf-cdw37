#include <stdio.h>
#include <stdbool.h>

#include "lexical.h"
#include "parser.h"
#include "types.h"
#include "symbols.h"
#include "echain.h"
#include "error.h"

void match(int i)
{
	// Throw possible error
	if (lookahead != i)
	{
		switch (i)
		{
		case ID:
		{
			error("Expected identifier.");
		}
		
		case NUM:
		{
			error("Expected r-value.");
		}
		
		case BEGIN:
		case END:
		{
			break;
		}
		
		case ';':
		case '(':
		case ')':
		{
			char buffer[32];

			snprintf(buffer, 32, "Expected control character '%c'.", (char) i);
			
			error(buffer);
		}
		
		default:
		{
			char buffer[64];
			
			snprintf(buffer, 64, "Unknown expected match during parsing! Target match is %d.", i);
			
			error(buffer);
		}
		}
	}
	
	// If properly matched, continue to the next token
	lookahead = lexan();
}

void factor()
{
	if (lookahead == ID)
	{
		// Match a known variable
		
		match(ID);
		
		if (lookup(value) == NOT_FOUND)
		{
			char buffer[32];
			
			snprintf(buffer, 32, "Undefined identifier '%s'.", value);
			
			error(buffer);
		}
	}
	else if (lookahead == NUM)
	{
		// Match a factor of the form 'number'
		
		match(NUM);
	}
	else if (lookahead == '(')
	{
		// Match a factor of the form '(expression)'
		
		match('(');
		
		expression();
		
		match(')');
	}
	else
	{
		// Failed to match a factor
		
		error("Expected identifier, r-value, or expression!");
	}
}

void term()
{
	// All terms start with a factor
	factor();
	
	while (lookahead == '*' || lookahead == '/')
	{
		// Match the following factors in the form of '*factor' or '/factor'

		match(lookahead);
		
		factor();
	}
}


void expression()
{
	// All expressions start with a term
	term();
	
	while (lookahead == '+' || lookahead == '-')
	{
		// Match the following terms in the form of '+term' or '-term'
		
		match(lookahead);
		
		term();
	}
}

void statement()
{
	// All statements start with a variable
	match(ID);
	
	// Add the variable to the symbol table if not already defined
	int pos = lookup(value);
	
	if (pos == NOT_FOUND)
	{
		pushSymbol(value, ID);
	}
	
	// Match the assignment in the form '=expression;'
	
	match('=');

	expression();
	
	match(';');
}

void parse()
{
	bool parsing = false;
	
	while (lookahead != DONE)
	{
		switch (lookahead)
		{
		case BEGIN:
		{
			// Throw an error if already parsing; otherwise, begin parsing

			if (parsing)
			{
				error("BEGIN statement already specified.");
			}
			
			match(BEGIN);
			
			parsing = true;
			
			break;
		}
		case ID:
		{
			// Throw an error if not parsing; otherwise, interpret statement
			
			if (parsing)
			{
				statement();
			}
			else
			{
				error("Statement outside of BEGIN and END clauses.");
			}
			
			break;
		}
		case NUM:
		{
			// Throw an error: you cannot assign a number to an expression
			
			error("An l-value cannot be a raw number!");
		}
		case END:
		{
			// Throw an error if not parsing; otherwise, end parsing
			
			if (!parsing)
			{
				error("END statement already specified or BEGIN never stated.");
			}
			
			match(END);
			
			parsing = false;
			
			break;
		}
		}
	}
	
	printf("Successfully parsed file!\n");
}
