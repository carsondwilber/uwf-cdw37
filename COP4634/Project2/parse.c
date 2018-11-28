#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "parse.h"

Param_t * parseArguments(char *buffer)
{
	Param_t *params = (Param_t *)malloc(sizeof(Param_t));
	char *delimiters = " \n\t";
	char *token = strtok(buffer, delimiters);
	
	params->argumentCount = 0;
	params->flagCount = 0;
	
	params->append[0] = false;
	params->append[1] = false;
	
	do
	{
		if (token[0] == '<')
		{
			params->redirects[0] = (char *)malloc(sizeof(char) * strlen(token) - 1);
			strcpy(params->redirects[1], token + 1);
		}
		else if (token[0] == '2')
		{
			if (token[1] == '>')
			{
				if (token[2] == '>')
				{
					params->redirects[2] = (char *)malloc(sizeof(char) * (strlen(token) - 2));
					strcpy(params->redirects[2], token + 2);
					params->append[1] = true;
				}
				else
				{
					params->redirects[2] = (char *)malloc(sizeof(char) * (strlen(token) - 1));
					strcpy(params->redirects[2], token + 1);
				}
			}
			else
			{
				params->argumentVector[params->argumentCount] = (char *)malloc(sizeof(char) * strlen(token));
				strcpy(params->argumentVector[params->argumentCount], token);
				params->argumentCount++;
			}
		}
		else if (token[0] == '>')
		{
			if (token[1] == '>')
			{
				params->redirects[1] = (char *)malloc(sizeof(char) * (strlen(token) - 2));
				strcpy(params->redirects[1], token + 2);
				params->append[0] = true;
			}
			else
			{
				params->redirects[1] = (char *)malloc(sizeof(char) * (strlen(token) - 1));
				strcpy(params->redirects[1], token + 1);
			}
		}
		else if (token[0] == '-')
		{
			if (token[1] == '-')
			{
				params->flagVector[params->flagCount] = (char *)malloc(sizeof(char) * (strlen(token) - 2));
				strcpy(params->flagVector[params->flagCount], token + 2);
			}
			else
			{
				params->flagVector[params->flagCount] = (char *)malloc(sizeof(char) * (strlen(token) - 1));
				strcpy(params->flagVector[params->flagCount], token + 1);
			}

			params->flagCount++;
		}
		else
		{
			params->argumentVector[params->argumentCount] = (char *)malloc(sizeof(char) * strlen(token));
			strcpy(params->argumentVector[params->argumentCount], token);
			params->argumentCount++;
		}
	} while ((token = strtok(NULL, delimiters)) != NULL);
	
	return params;
}

bool checkFlag(char **vector, int size, char *flag)
{
	int i;
	for (i = 0; i < size; ++i)
	{
		if (strcmp(vector[i], flag) == 0)
		{
			return true;
		}
	}
	
	return false;
}
