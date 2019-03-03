/*
 * File: http-craft.h
 * Created: 17 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides an interface for crafting HTTP responses.
 */

#ifndef HTTP_CRAFT_H
#define HTTP_CRAFT_H

/**
 * Description: 
 * 	This is a builder function for building an http field given the parameters passed by the caller
 * this function returns the total length of the http field
 *
 * int fieldId:    The field id type
 * const char *fieldValue:	The field value of what is being created
 * char *data:	The data being placed in the field
 * int maxBytes:	The max bytes alloted to the field being created, this is enforced
 */
int craftHTTPField(const char *fieldName, const char *fieldValue, char *data, int maxBytes);

/**
 * Description: 
 * 	This is a builder function that will create an http response header and returns the total length
 *
 * const char *httpVersion:	The http version that the header is designed for
 * const int responseCode:	This is the response code being sent
 * char *data:	The data being sent
 * int maxBytes:	The maximum bytes being sent, this is enforced
 */
int craftHTTPResponseHeader(const char *httpVersion, const int responseCode, char *data, int maxBytes);

/**
 * Description: 
 * 	This is a builder function that will create a standard error response and return the header buffer
 *
 * int responseCode:	This is the response code being sent
 */
char * craftStandardErrorResponse(int responseCode);

/**
 * Description: 
 * 	This is a builder function that will create a resource response and return the header buffer char array
 *
 * char *contentType:	This dictates the content type
 * int contentLength:	The total length of the content
 */
char * craftStandardResourceResponse(char *contentType, int contentLength);

#endif // HTTP_CRAFT_H
