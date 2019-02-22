#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "file.h"
#include "lexical.h"
#include "symbols.h"
#include "echain.h"
#include "input.h"
#include "error.h"

char ch;

char *value;

char *numLexeme;

int lookahead;

void identifier()
{
	// Free the current variable name if not empty
	if (value)
	{
		free(value);
	}
	
	// Initilize 10 characters of space for the name
	int l = 10;
	value = (char *)malloc(sizeof(char) * l);
	
	// Set the first character to the currently read character
	value[0] = ch;
	
	// Read the second character
	ch = advance();
	
	// Start the following loop with the second character
	int cl = 1;

	// Continue reading characters while they are valid (A-Z, a-z, 0-9, _)
	while ((ch >= 'a' && ch <= 'z') ||
		(ch >= 'A' && ch <= 'Z') ||
		(ch >= '0' && ch <= '9') ||
		ch == '_')
	{
		// Reject variable names that have consecutive underscores
		if (ch == '_' && value[cl - 1] == '_')
		{
			error("Identifiers may not have more than one consecutive underscore.");
		}
		
		// Allocate more space if the variable name is long
		if (cl == l - 2)
		{
			l *= 2;
			char *copy = (char *)malloc(sizeof(char) * l);
			strncpy(copy, value, l / 2);
			free(value);
			value = copy; 
		}
		
		// Read the current character into the variable name
		value[cl++] = ch;
		
		// Get the next character
		ch = advance();
	}
	
	// Rewind one character after reading the whole variable name
	fseek(inputFile, -1, SEEK_CUR);
	
	// Reject variable names that end in underscores
	if (value[cl - 1] == '_')
	{
		error("Identifiers may not end in underscores.");
	}
	
	// Ensure the variable name properly terminates
	value[cl] = '\0';
}

void number()
{
	// Free the current number token if not empty
	if (numLexeme)
	{ 
		free(numLexeme);
	}
	
	// Initialize 10 characters of space for the number
	int l = 10;
	numLexeme = (char *)malloc(sizeof(char) * l);
	
	// Start the following loop at the first character
	int cl = 0;
	
	// Assume the number is an integer until proven a floating point
	bool decimal = false;
	
	// Continue reading characters while they are valid (0-9, .)
	do
	{
		// Allocate more space if the number is long
		if (cl == l)
		{
			l *= 2;
			char *copy = (char *)malloc(sizeof(char) * l);
			strncpy(copy, numLexeme, l / 2);
			free(numLexeme);
			numLexeme = copy; 
		}
		
		// Read the current character into the number token
		numLexeme[cl++] = ch;
		
		// Get the next character
		ch = advance();
		
		// Consider flipping the decimal flag if the next character
		// is a period
		if (ch == '.')
		{
			// Reject numbers with two decimal points
			if (decimal)
			{
				error("Number cannot have two decimal places!");
			}
			
			decimal = true;
		}
	} while ((ch >= '0' && ch <= '9') || ch == '.');
	
	// Rewind one character after reading the whole number token
	fseek(inputFile, -1, SEEK_CUR);
	
	// Ensure the number token properly terminates
	numLexeme[cl] = '\0';
}

int lexan()
{
	while (true)
	{
		ch = advance();
		
		if (ch == ' ' || ch == '\t')
		{
			// Ignore whitespace
			
			continue;
		}
		else if (ch == '\n')
		{
			// Advance to the next line and increment the debug line tracker
			
			lineno++;
			
			charno = 1;
			
			continue;
		}
		else if (ch >= '0' && ch <= '9')
		{
			// Interpret the token as a number if it begins with one
			
			number();
			
			return NUM;
		}
		else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		{
			// Interpret the token as an identifier if it begins with a letter

			identifier();
			
			// See if the identifier has already been defined
			int pos = lookup(value);
			
			if (pos == NOT_FOUND)
			{
				// If not, add it to the symbol table
				
				identify(value);
			}
			else if (pos == 0)
			{
				// Return the BEGIN flag if it is a 'begin' token
				
				return BEGIN;
			}
			else if (pos == 1)
			{
				// Return the END flag if it is an 'end' token
				
				return END;
			}
			
			// Return the identifier if not 'begin' or 'end'
			return ID;
		}
		else if (ch == '~')
		{
			// Ignore comments

			while (ch != '\n') ch = advance();
			
			lineno++;

			charno = 0;
			
			continue;
		}
		else if (ch == EOF)
		{
			// Return the DONE flag to indicate successful lexical analysis!

			return DONE;
		}
		
		// In any unknown case, return the character itself
		return (int) ch;
	}
}
