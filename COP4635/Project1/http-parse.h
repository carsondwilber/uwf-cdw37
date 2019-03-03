/*
 * File: http-parse.h
 * Created: 17 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides an interface for parsing HTTP requests/responses.
 */

#include "http.h"

#ifndef HTTP_PARSE_H
#define HTTP_PARSE_H

/**
 * Description: 
 * 	This is a parsing function that will parse a weighted http field and returns if the parse was a 
 * success or a failure
 *
 * int fieldId:    The field id type that is to be parsed
 * char* fieldValue:	The value of the field to be returned
 * char*** tokens:	The tokens to be returned
 * double** weights:	Returns the weights of the token language
 */
int parseHTTPFieldMultipartWeightedValue(int fieldId, char *fieldValue, char ***tokens, double **weights);

/**
 * Description: 
 * 	This is a parsing function that will parse a non-weighted http field and returns if the parse was a 
 * success or a failure
 *
 * int fieldId:    The field id type that is to be parsed
 * char* fieldValue:	The value of the field to be returned
 * char*** tokens:	The tokens to be returned
 */
int parseHTTPFieldMultipart(int fieldId, char *fieldValue, char ***tokens);

/**
 * 				/// IN PROGRESS ///
 * Description: 
 * 	This is a parsing function that will parse an http text field
 *
 * int fieldId:    The field id type that is to be parsed
 * char* fieldValue:	The value of the field to be returned
 */
int parseHTTPFieldTextValue(int fieldId, char *fieldValue);

/**
 * 				/// IN PROGRESS ///
 * Description: 
 * 	This is a parsing function that will parse a given integer field 
 *
 * int fieldId:    The field id type that is to be parsed
 * char* fieldValue:	The value of the field to be returned
 * int *value:	The tokens to be returned
 */
int parseHTTPFieldIntegerValue(int fieldId, char *fieldValue, int *value);

/**
 * Description: 
 * 	This is a parsing function that will parse a field name
 *		Example: "Content-Type", "Content-Length", et cetera
 *
 * char *fieldId:    The field id type that is to be parsed and returns the field name value
 */
int parseHTTPFieldName(char *fieldName);

/**
 * Description: 
 * 	This is a parsing function that will parse the request header of given data and returns the gathered
 * data and an integer for the type of error encountered, if there is one
 *
 * char *data: The data that is parsed from the header
 * char **requestType:	The request type that was sent
 * char **resource:	The resource types encountered in the header
 * char **httpVersion:	The http version encountered in the header
 */
int parseHTTPRequestHeader(char *data, char **requestType, char **resource, char **httpVersion);

#endif // HTTP_PARSE_H
