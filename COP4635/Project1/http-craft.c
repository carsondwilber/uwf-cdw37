#include <string.h>
#include <stdio.h>

#include "http-util.h"
#include "logging.h"
#include "http.h"
#include "http-craft.h"

int craftHTTPField(const char *fieldName, const char *fieldValue, char *data, int maxBytes)
{
	int totalLen = strlen(fieldName) + strlen(fieldValue) + 4;
	
	if (totalLen >= maxBytes)
	{
		return CRAFT_FAILED;
	}
	
	if (!validateFieldValue(getHTTPFieldValueType(getHTTPFieldId(fieldName)), fieldValue))
	{
		return INVALID_VALUE;
	}
	
	snprintf(data, totalLen + 1, "%s: %s\r\n", fieldName, fieldValue);
	
	return totalLen;
}

int craftHTTPResponseHeader(const char *httpVersion, const int responseCode, char *data, int maxBytes)
{
	if (!validateHTTPVersion(httpVersion))
	{
		return UNKNOWN_VERSION;
	}
	
	if (!validateHTTPResponseCode(responseCode))
	{
		return UNKNOWN_CODE;
	}
	
	char prebuffer[48] = {0};
	
	char *responseDescription = getHTTPResponseDescription(responseCode);
	
	int versionBytes = strlen(httpVersion);
	
	int descriptionBytes = strlen(responseDescription);
	
	snprintf(prebuffer, 47, "HTTP/%%%ds %d %%%ds\r\n", versionBytes, responseCode, descriptionBytes);
	
	char buffer[48] = {0};
	
	snprintf(buffer, 47, prebuffer, httpVersion, responseDescription);
	
	if (strlen(buffer) >= maxBytes)
	{
		log_msg(ERROR, "Response data too large for buffer!");
		
		return CRAFT_FAILED;
	}
	
	snprintf(data, strlen(buffer) + 1, "%s", buffer);
	
	return strlen(buffer);
}
