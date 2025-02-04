/*
 * File: http.h
 * Created: 14 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides preprocessor definitions for HTTP error codes and messages, as well as some parser values.
 */

#ifndef HTTP_H
#define HTTP_H

/**********     HTTP RESPONSE CODES     **********/

/*****    SUCCESS 2xx    *****/

#define HTTP_SUCCESS_200_OK				200

#define HTTP_SUCCESS_200_OK_MESSAGE			"OK"

/***** CLIENT ERRORS 4xx *****/

#define HTTP_ERROR_400_BAD_REQUEST 			400
#define HTTP_ERROR_404_NOT_FOUND 			404

#define HTTP_ERROR_400_BAD_REQUEST_MESSAGE		"Bad Request"
#define HTTP_ERROR_404_NOT_FOUND_MESSAGE		"Not Found"

/***** SERVER ERRORS 5xx *****/

#define HTTP_ERROR_500_INTERNAL_SERVER_ERROR 		500
#define HTTP_ERROR_501_NOT_IMPLEMENTED 			501
#define HTTP_ERROR_505_VERSION_NOT_SUPPORTED 		505

#define HTTP_ERROR_500_INTERNAL_SERVER_ERROR_MESSAGE	"Internal Server Error"
#define HTTP_ERROR_501_NOT_IMPLEMENTED_MESSAGE		"Not Implemented"
#define HTTP_ERROR_505_VERSION_NOT_SUPPORTED_MESSAGE	"Version Not Supported"

/**********     FIELD NAME DEFINES      **********/

#define UNKNOWN_FIELD					-80
#define IGNORED_FIELD					-81

#define HTTP_FIELD_HOST					800
#define HTTP_FIELD_CONTENT_TYPE				801
#define HTTP_FIELD_CONTENT_LENGTH			802
#define HTTP_FIELD_CONNECTION				803
#define HTTP_FIELD_USER_AGENT				804
#define HTTP_FIELD_ACCEPT_LANGUAGE			805
#define HTTP_FIELD_CONTENT_LANGUAGE			806

/**********     FIELD VALUE DEFINES     **********/

#define PARSE_FAILED					-82
#define EMPTY_VALUE					-83
#define INVALID_VALUE					-84
#define UNKNOWN_VALUE					-85
#define IGNORED_VALUE					-86

#define TEXT_VALUE					10
#define MULTIPART_VALUE_COMMAS				11
#define MULTIPART_VALUE_SEMICOLONS			12
#define MULTIPART_WEIGHTED_VALUE			13
#define INTEGER_VALUE					14

#define HTTP_CONTENT_TYPE_TEXT_HTML			900
#define HTTP_CONTENT_ACCEPT_LANGUAGE_EN			901
#define HTTP_CONTENT_ACCEPT_LANGUAGE_MULTIPLE		902
#define HTTP_CONTENT_ACCEPT_LANGUAGE_OTHER		903
#define HTTP_CONTENT_LANGUAGE_EN			904
#define HTTP_CONTENT_LANGUAGE_MULTIPLE			905
#define HTTP_CONTENT_LANGUAGE_OTHER			906
#define HTTP_CONNECTION_KEEP_ALIVE			907
#define HTTP_CONNECTION_MULTIPLE			908
#define HTTP_CONNECTION_OTHER				909

/**********    REQUEST HEADER VALUES    **********/

#define INVALID_HEADER					-87
#define UNKNOWN_METHOD					-88
#define UNKNOWN_VERSION					-89
#define UNKNOWN_CODE					-90

#define VALID_HEADER					910

/**********       DEFAULT VALUES        **********/

#define DEFAULT_VERSION					"1.1"

/**********        OTHER DEFINES        **********/

#define CRAFT_FAILED					-91

#endif // HTTP_H
