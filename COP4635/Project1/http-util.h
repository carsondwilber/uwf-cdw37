/*
 * File: http-util.h
 * Created: 21 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides an interface for validating field values and generating valid requests/responses.
 */

#include <stdbool.h>

#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

/*
 * Specifies the delimiter for commas
 */
extern const char multipart_commas_delim[];

/*
 * Specifies the delimiter for semicolons
 */
extern const char multipart_semicolons_delim[];

/*
 * Specifies the delimiter for single semicolons
 */
extern const char weighted_delim[];

/*
 * Specifies the delimiter for identifying spacial locality
 */
extern const char locality_delim[];

/*
 * Specifies all language code abbreviations
 */
extern const char *language_codes[];

/**
 * Description: 
 * 	This is a utility function that will set validate that the given character array is an integer and
 * returns true or false depending on what is detected.
 *
 * const char *ptr:    This is a character array for what is to be checked
 */
bool validateInteger(const char *ptr);

/**
 * Description: 
 * 	This is a utility function that will validate that a language weight conforms to the proper format
 * 1
 * 0.#... 
 * .#...
 * 
 * const char *ptr:    This is a character array for what is to be checked
 */
bool validateWeight(const char *ptr);

/**
 * Description: 
 * 	This is a utility function that will take a character string and check if it is a valid language code 
 *
 * const char *code:    This is a character array for what is to be checked
 */
bool validateLanguageCode(const char *code);

/**
 * Description: 
 * 	This is a utility function that will 
 *
 * const char *httpVersion:    This is a character array for what is to be checked
 */
bool validateHTTPVersion(const char *httpVersion);

/**
 * Description: 
 * 	This is a utility function that will validate an http response code and return true if valid else false
 *		Example: HTTP_SUCCESS_200, HTTP_ERROR_400_BAD_REQUEST, et cetera.
 *
 * int responseCode:    This is a character array for what is to be checked
 */
bool validateHTTPResponseCode(int responseCode);

/**
 * 					/// IN PROGRESS ///
 * Description: 
 * 	This is a utility function that will validate a field value based on its id and given field value
 *
 * int fieldID:	The given identifier for the field type
 * const char *fieldValue:    This is a character array giving what was inside the field value
 */
bool validateFieldValue(int fieldId, const char *fieldValue);

/**
 * Description: 
 * 	This is a utility function that will return the HTTP field id type
 *		Example: HTTP_FIELD_HOST, HTTP_FIELD_CONTENT_TYPE, et cetera.
 *
 * const char *fieldName:    This is a character array for what is to be evaluated
 */
int getHTTPFieldId(const char *fieldName);

/**
 * Description: 
 * 	This is a utility function that will return the HTTP field value type
 *		Example: HTTP_FIELD_USER_AGENT, HTTP_FIELD_ACCEPT_LANGUAGE, et cetera.
 *
 * int fieldId:    This is a character array for what is to be checked
 */
int getHTTPFieldValueType(int fieldId);

/**
 * Description: 
 * 	This is a utility function that will return the HTTP response description as a character array
 *		Example: HTTP_SUCCESS_200_OK, HTTP_ERROR_400_BAD_REQUEST, et cetera.
 *
 * int responseCode:    This is a character array for what is to be checked
 */
char * getHTTPResponseDescription(int responseCode);

#endif // HTTP_UTIL_H
