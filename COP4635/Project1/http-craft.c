/*
 * File: http-craft.c
 * Created: 17 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides implementation for crafting HTTP responses.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "filetypes.h"
#include "http-util.h"
#include "logging.h"
#include "http.h"
#include "http-craft.h"

int craftHTTPField(const char *fieldName, const char *fieldValue, char *data, int maxBytes)
{
	int totalLen = strlen(fieldName) + strlen(fieldValue) + 4;
	
	// Enforce maxBytes limit.
	if (totalLen >= maxBytes)
	{
		return CRAFT_FAILED;
	}
	
	// Enforce only valid values for this field.
	if (!validateFieldValue(getHTTPFieldValueType(getHTTPFieldId(fieldName)), fieldValue))
	{
		return INVALID_VALUE;
	}
	
	snprintf(data, totalLen + 1, "%s: %s\r\n", fieldName, fieldValue);
	
	return totalLen;
}

int craftHTTPResponseHeader(const char *httpVersion, const int responseCode, char *data, int maxBytes)
{
	// Enforce only valid HTTP versions.
	if (!validateHTTPVersion(httpVersion))
	{
		return UNKNOWN_VERSION;
	}
	
	// Enforce only valid HTTP codes.
	if (!validateHTTPResponseCode(responseCode))
	{
		return UNKNOWN_CODE;
	}
	
	// Produce a pre-buffer for the header for strict sizing: HTTP/() () ()
	
	char prebuffer[48] = {0};
	
	char *responseDescription = getHTTPResponseDescription(responseCode);
	
	int versionBytes = strlen(httpVersion);
	
	int descriptionBytes = strlen(responseDescription);
	
	snprintf(prebuffer, 47, "HTTP/%%%ds %d %%%ds\r\n", versionBytes, responseCode, descriptionBytes);
	
	// Use the pre-buffer as a format string and fill in the format specifiers.
	
	char buffer[48] = {0};
	
	snprintf(buffer, 47, prebuffer, httpVersion, responseDescription);
	
	// Enforce maxBytes limit.
	if (strlen(buffer) >= maxBytes)
	{
		log_msg(ERROR, "Response data too large for buffer!");
		
		return CRAFT_FAILED;
	}
	
	snprintf(data, strlen(buffer) + 1, "%s", buffer);
	
	return strlen(buffer);
}

char * craftStandardErrorResponse(int responseCode)
{
	// Allocate an error response of a maximum of 128 bytes.
	
	char *responseBuffer = (char *)calloc(256, sizeof(char));
	
	int totalBytes = 0;
	
	// Craft the header.
	{
		int headerBytes = craftHTTPResponseHeader(DEFAULT_VERSION, responseCode, responseBuffer, 128);
		
		if (headerBytes == UNKNOWN_VERSION || headerBytes == UNKNOWN_CODE || headerBytes == CRAFT_FAILED)
		{
			log_msg(ERROR, "Could not craft standard error response!");
			
			return NULL;
		}
		
		totalBytes += headerBytes;
	}
	
	// Craft the Connection field with Keep-Alive.
	{
		int connectionBytes = craftHTTPField("Connection", "Keep-Alive", responseBuffer + totalBytes, 128 - totalBytes);
		
		if (connectionBytes == CRAFT_FAILED || connectionBytes == INVALID_VALUE)
		{
			log_msg(ERROR, "Could not craft standard error response! Failed to create Connection header field.");
			
			return NULL;
		}
		
		totalBytes += connectionBytes;
	}
	
	
	// Craft the Content-Type field with the given contentType.
	{
		int contentTypeBytes = craftHTTPField("Content-Type", FILETYPE_TEXT_HTML, responseBuffer + totalBytes, 256 - totalBytes);
		
		if (contentTypeBytes == CRAFT_FAILED || contentTypeBytes == INVALID_VALUE)
		{
			log_msg(ERROR, "Could not craft standard resource response! Failed to create Content-Type header field.");
			
			return NULL;
		}
		
		totalBytes += contentTypeBytes;
	}
	
	char *httpResponseDescription = getHTTPResponseDescription(responseCode);
	
	// Append the Content-Length field given the known contentLength.
	snprintf(responseBuffer + totalBytes, 256 - totalBytes, "Content-Length: %d\r\n\r\n%d %s", ((int) strlen(httpResponseDescription)) + 4, responseCode, httpResponseDescription);
	
	// Return the completed buffer (or NULL if an error state was produced above.)
	
	return responseBuffer;
}

char * craftStandardResourceResponse(char *contentType, int contentLength)
{
	// Allocate a resource response of a maximum of 256 bytes.
	
	char * headersBuffer = (char *)calloc(256, sizeof(char));
	
	int totalBytes = 0;
	
	// Craft the header.
	int headerBytes = craftHTTPResponseHeader(DEFAULT_VERSION, HTTP_SUCCESS_200_OK, headersBuffer, 256);
	
	if (headerBytes == UNKNOWN_VERSION || headerBytes == UNKNOWN_CODE || headerBytes == CRAFT_FAILED)
	{
		log_msg(ERROR, "Could not craft standard resource response!");
		
		return NULL;
	}
	
	totalBytes += headerBytes;
	
	// Craft the Connection field with Keep-Alive.
	int connectionBytes = craftHTTPField("Connection", "Keep-Alive", headersBuffer + totalBytes, 256 - totalBytes);
	
	if (connectionBytes == CRAFT_FAILED || connectionBytes == INVALID_VALUE)
	{
		log_msg(ERROR, "Could not craft standard resource response! Failed to create Connection header field.");
		
		return NULL;
	}
	
	totalBytes += connectionBytes;
	
	// Craft the Content-Type field with the given contentType.
	int contentTypeBytes = craftHTTPField("Content-Type", contentType, headersBuffer + totalBytes, 256 - totalBytes);
	
	if (contentTypeBytes == CRAFT_FAILED || contentTypeBytes == INVALID_VALUE)
	{
		log_msg(ERROR, "Could not craft standard resource response! Failed to create Content-Type header field.");
		
		return NULL;
	}
	
	totalBytes += contentTypeBytes;
	
	// Append the Content-Length field given the known contentLength.
	snprintf(headersBuffer + totalBytes, 256 - totalBytes, "Content-Length: %d\r\n", contentLength);
	
	// Return the completed buffer (or NULL if an error state was produced above.)
	
	return headersBuffer;
}
