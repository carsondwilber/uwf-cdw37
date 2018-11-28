#include "parse.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Param_t * parseArguments(char *buffer)
{
	Param_t *params = (Param_t *)malloc(sizeof(Param_t));
	char *delimiters = " \n\t";
	char *token = strtok(buffer, delimiters);
	
	params->argumentCount = 0;
	
	do
	{
		if (token[0] == '<')
		{
			params->inputRedirect = (char *)malloc(sizeof(char) * strlen(token) - 1);
			strcpy(params->inputRedirect, token + 1);
		}
		else if (token[0] == '>')
		{
			params->outputRedirect = (char *)malloc(sizeof(char) * strlen(token) - 1);
			strcpy(params->outputRedirect, token + 1);
		}
		else
		{
			params->argumentCount++;
			params->argumentVector[params->argumentCount - 1] = (char *)malloc(sizeof(char) * strlen(token));
			strcpy(params->argumentVector[params->argumentCount - 1], token);
		}
	} while ((token = strtok(NULL, delimiters)) != NULL);
	
	return params;
}
