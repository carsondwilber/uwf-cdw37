#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "util.h"
#include "http.h"
#include "http-parse.h"
#include "http-util.h"
#include "logging.h"

int parseHTTPFieldMultipartWeightedValue(int fieldId, char *fieldValue, char ***tokens, double **weights)
{
	if (tokens)
	{
		log_msg(ERROR, "Cannot determine valid field value with tokens already allocated.");
		
		return PARSE_FAILED;
	}
	
	switch (fieldId)
	{
	case HTTP_FIELD_ACCEPT_LANGUAGE:
	{
		char *fieldValueCopy = (char *)malloc(sizeof(char) * (strlen(fieldValue) + 1));
	
		strncpy(fieldValueCopy, fieldValue, strlen(fieldValue));
		
		fieldValueCopy[strlen(fieldValue)] = '\0';
		
		*tokens = (char **)malloc(sizeof(char *));
		
		char *ptr = strtok(fieldValueCopy, multipart_commas_delim);
		
		if (ptr == NULL)
		{
			log_msg(ERROR, "An empty value cannot be uesd for Accept-Language!");
			
			return EMPTY_VALUE;
		}
		
		int nTokens = 1;
		
		(*tokens)[0] = (char *)malloc(sizeof(char) * (strlen(ptr) + 1));
		strncpy((*tokens)[0], ptr, strlen(ptr));
		(*tokens)[0][strlen(ptr)] = '\0';
		
		ptr = strtok(NULL, multipart_commas_delim);
		
		while (ptr != NULL)
		{
			*tokens = (char **)realloc(*tokens, sizeof(char *) * ++nTokens);
			*weights = (double *)realloc(*weights, sizeof(double) * ++nTokens);
			
			if (*tokens == NULL || *weights == NULL)
			{
				log_msg(ERROR, "Cannot allocate enough memory to parse Accept-Language field!");
				
				return PARSE_FAILED;
			}
			
			unsigned int len = strlen(ptr);
			
			char *tmp = (char *)malloc(sizeof(char) * (len + 1));
			strncpy(tmp, ptr, len);
			tmp[len] = '\0';
			
			char *tmptr = strtok(tmp, weighted_delim);
			unsigned int tmplen = strlen(tmptr);
			
			if (validateLanguageCode(ptr))
			{
				if (tmplen < len)
				{
					if (validateWeight(tmptr + 2))
					{
						(*weights)[nTokens - 1] = atof(tmptr + 2);
					}
				}
				else
				{
					char buffer[40];
					
					snprintf(buffer, 39, "Weight for language %3s is default (1).", ptr);
					
					buffer[39] = '\0';
					
					log_msg(INFO, buffer);
					
					(*weights)[nTokens - 1] = 1.0;
				}
				
				(*tokens)[nTokens - 1] = (char *)malloc(sizeof(char) * (strlen(ptr) + 1));
				strncpy((*tokens)[nTokens - 1], ptr, strlen(ptr));
				(*tokens)[nTokens - 1][strlen(ptr) - 1] = '\0';
				
				ptr = strtok(NULL, multipart_commas_delim);
				
				continue;
			}
			
			if (len > 8)
			{
				log_msg(ERROR, "Item for Accept-Language is unknown and the string is too long!");
			}
			else
			{
				char *buffer = (char *)malloc(sizeof(char) * (len + 42));
				
				snprintf(buffer, len + 42, "Item for Accept-Language '%s' is not valid!", ptr);
				
				buffer[41] = '\0';
				
				log_msg(ERROR, buffer);
				
				free(buffer);
			}
			
			log_msg(WARN, "Freeing tokens and weights for Accept-Language and returning INVALID_VALUE.");
			
			unsigned int i;
			
			for (i = 0; i < nTokens - 1; ++i)
			{
				free((*tokens)[i]);
			}
			
			free(*tokens);
			
			free(*weights);
			
			return INVALID_VALUE;
		}
		
		log_msg(INFO, "Checking for EN among accepted languages.");
		
		unsigned int i = 0;
		
		while (i < nTokens && ((*tokens)[i][0] != 'e' || (*tokens)[i][1] != 'n'))
		{
			i++;
		}
		
		if (i < nTokens)
		{
			if (nTokens == 1)
			{
				log_msg(INFO, "EN is the only accepted language.");
				
				return HTTP_CONTENT_ACCEPT_LANGUAGE_EN;
			}
			
			log_msg(INFO, "EN is an accepted language, among others.");
			
			return HTTP_CONTENT_ACCEPT_LANGUAGE_MULTIPLE;
		}
		
		log_msg(WARN, "EN is not an accepted language.");
		
		return HTTP_CONTENT_ACCEPT_LANGUAGE_OTHER;
	}
	
	default: break;
	}
	
	return UNKNOWN_FIELD;
}

int parseHTTPFieldMultipart(int fieldId, char *fieldValue, char ***tokens)
{
	int fieldType = getHTTPFieldValueType(fieldId);
	
	switch (fieldType)
	{
	case MULTIPART_VALUE_COMMAS:
	{
		char *fieldValueCopy = (char *)malloc(sizeof(char) * (strlen(fieldValue) + 1));
		
		strncpy(fieldValueCopy, fieldValue, strlen(fieldValue));
		
		fieldValueCopy[strlen(fieldValue)] = '\0';
		
		*tokens = (char **)malloc(sizeof(char *));
		
		switch (fieldId)
		{
		case HTTP_FIELD_CONNECTION:
		{
			char *ptr = strtok(fieldValueCopy, multipart_commas_delim);
			
			int nTokens = 1;
			
			(*tokens)[0] = (char *)malloc(sizeof(char) * (strlen(ptr) + 1));
			strncpy((*tokens)[0], ptr, strlen(ptr));
			(*tokens)[0][strlen(ptr)] = '\0';
			
			bool keepAlive = false;
			
			if (strcicmp((*tokens)[0], "Keep-Alive") == 0)
			{
				log_msg(INFO, "Connection will be kept alive.");
				
				keepAlive = true;
			}
			
			ptr = strtok(NULL, multipart_commas_delim);
			
			while (ptr != NULL)
			{
				*tokens = (char **)realloc(tokens, sizeof(char *) * ++nTokens);
				
				if (*tokens == NULL)
				{
					log_msg(ERROR, "Cannot allocate enough memory to parse Connection field!");
					
					return PARSE_FAILED;
				}
				
				unsigned int len = strlen(ptr);
				
				char *tmp = (char *)malloc(sizeof(char) * (len + 1));
				strncpy(tmp, ptr, len);
				tmp[len] = '\0';
				
				if (!keepAlive)
				{
					if (strcicmp((*tokens)[0], "Keep-Alive"))
					{
						log_msg(INFO, "Connection will be kept alive.");
						
						keepAlive = true;
					}
				}
			}
			
			if (keepAlive)
			{
				if (nTokens > 1)
				{
					return HTTP_CONNECTION_MULTIPLE;
				}
				
				return HTTP_CONNECTION_KEEP_ALIVE;
			}
			
			return HTTP_CONNECTION_OTHER;
		}
		
		default: break;
		}
		
		return UNKNOWN_FIELD;
	}
	
	case MULTIPART_VALUE_SEMICOLONS:
	{
		switch (fieldId)
		{
		case HTTP_FIELD_CONTENT_TYPE:
		{
			return IGNORED_FIELD;
		}
		
		default: break;
		}
		
		return UNKNOWN_FIELD;
	}
	
	default: break;
	}
	
	log_msg(ERROR, "Unknown field value attempted to parse as multipart value!");
	
	return INVALID_VALUE;
}

int parseHTTPFieldIntegerValue(int fieldId, char *fieldValue, int *value)
{
	switch (fieldId)
	{
	case HTTP_FIELD_CONTENT_LENGTH:
	{
		/*if (!validateInteger(fieldValue))
		{
			return INVALID_VALUE;
		}
		
		int parsedValue = atoi(fieldValue);
		
		if (parsedValue < 0)
		{
			return INVALID_VALUE;
		}
		
		return INTEGER_VALUE;*/
		
		return IGNORED_FIELD;
	}
	}
	
	return UNKNOWN_FIELD;
}

int parseHTTPFieldTextValue(int fieldId, char *fieldValue)
{
	switch (fieldId)
	{
	case HTTP_FIELD_USER_AGENT:
	{
		return IGNORED_VALUE;
	}
	}
	
	return UNKNOWN_FIELD;
}

int parseHTTPFieldName(char *fieldName)
{
	if (strcicmp(fieldName, "Host") == 0)
	{
		return HTTP_FIELD_HOST;
	}
	else if (strcicmp(fieldName, "Content-Type") == 0)
	{
		return HTTP_FIELD_CONTENT_TYPE;
	}
	else if (strcicmp(fieldName, "Content-Length") == 0)
	{
		return HTTP_FIELD_CONTENT_LENGTH;
	}
	else if (strcicmp(fieldName, "Connection") == 0)
	{
		return HTTP_FIELD_CONNECTION;
	}
	else if (strcicmp(fieldName, "User-Agent") == 0)
	{
		return HTTP_FIELD_USER_AGENT;
	}
	else if (strcicmp(fieldName, "Accept-Language") == 0)
	{
		return HTTP_FIELD_ACCEPT_LANGUAGE;
	}
	else if (strcicmp(fieldName, "Content-Language") == 0)
	{
		return HTTP_FIELD_CONTENT_LANGUAGE;
	}
	
	return UNKNOWN_FIELD;
}

int parseHTTPRequestHeader(char *data, char **requestType, char **resource, char **httpVersion)
{
	char *dataCopy = (char *)malloc(sizeof(char) * (strlen(data) + 1));
	
	strncpy(dataCopy, data, strlen(data));
	
	dataCopy[strlen(data)] = '\0';
	
	char *ptr = strtok(dataCopy, " ");
	
	if (ptr == NULL)
	{
		log_msg(ERROR, "Request header is empty!");
		
		return INVALID_HEADER;
	}
	
	*requestType = (char *)malloc(sizeof(char) * (strlen(ptr) + 1));
	
	strncpy(*requestType, ptr, strlen(ptr));
	
	(*requestType)[strlen(ptr)] = '\0';
	
	if (strcicmp(*requestType, "GET") != 0)
	{
		log_msg(ERROR, "Request method is not supported.");
		
		free(*requestType);
		
		return UNKNOWN_METHOD;
	}
	
	ptr = strtok(NULL, " ");
	
	if (ptr == NULL)
	{
		log_msg(ERROR, "Request header is missing resource!");
		
		free(*requestType);
		
		return INVALID_HEADER;
	}
	
	*resource = (char *)malloc(sizeof(char) * (strlen(ptr) + 1));
	
	strncpy(*resource, ptr, strlen(ptr));
	
	(*resource)[strlen(ptr)] = '\0';
	
	// TODO: Should we check if the resource path is a valid path? This can get complicated.
	
	ptr = strtok(NULL, " \r\n");
	
	char *tmp = strtok(ptr, "/");
	
	if (tmp == NULL)
	{
		log_msg(ERROR, "Request header HTTP version is not valid!");
		
		free(*requestType);
		
		free(*resource);
		
		return INVALID_HEADER;
	}
	
	tmp = strtok(NULL, " \r\n");
	
	if (tmp == NULL)
	{
		log_msg(ERROR, "Request header HTTP version is not valid!");
		
		free(*requestType);
		
		free(*resource);
		
		return INVALID_HEADER;
	}
	
	*httpVersion = (char *)malloc(sizeof(char) * (strlen(tmp) + 1));
	
	strncpy(*httpVersion, tmp, strlen(tmp));
	
	(*httpVersion)[strlen(tmp)] = '\0';
	
	if (strcicmp(*httpVersion, "1.1") != 0)
	{
		if (strlen(tmp) < 4)
		{
			char buffer[53];
			
			snprintf(buffer, 52, "Request header HTTP version '%3s' is not supported.", *httpVersion);
			
			log_msg(ERROR, buffer);
		}
		else
		{
			log_msg(ERROR, "Request header HTTP version is not supported and is too long to print.");
		}
		
		free(*requestType);
		
		free(*resource);
		
		free(*httpVersion);
		
		return UNKNOWN_VERSION;
	}
	
	ptr = strtok(NULL, " \r\n");
	
	if (ptr != NULL)
	{
		log_msg(ERROR, "Unknown extra information in request header!");
		
		free(*requestType);
		
		free(*resource);
		
		free(*httpVersion);
		
		return INVALID_HEADER;
	}
	
	return VALID_HEADER;
}
