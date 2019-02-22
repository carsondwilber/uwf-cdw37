#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "http.h"
#include "http-util.h"

const char multipart_commas_delim[] = " ,";
const char multipart_semicolons_delim[] = " ;";
const char weighted_delim[]  = ";";
const char locality_delim[] = "-";
const char *language_codes[] =  { "af",  "ar", "as", "ast", "az", "bg", "be", "bn",  "br", "bs",
			    "ca",  "ce", "ch",  "co", "cr", "cs", "cv", "cy",  "de", "en", 
			    "el",  "eo", "es",  "et", "eu", "fa", "fi", "fj",  "fo", "fr",
			   "fur",  "fy", "ga",  "gd", "gl", "gu", "he", "hi", "hsb", "ht",
			    "hu",  "hy", "id",  "is", "it", "iu", "ja", "ji",  "ka", "kk",
			    "km",  "kn", "ko",  "ks", "ky", "la", "lb", "lt",  "lv", "mi",
			    "mk",  "ml", "mo",  "mr", "ms", "mt", "my", "nb",  "ne", "ng",
			    "nn",  "no", "nv",  "oc", "om", "or", "pa", "pl",  "pt", "qu",
			    "rm",  "ro", "ru",  "sa", "sb", "sc", "sd", "sg",  "si", "sk",
			    "sl",  "so", "sq",  "sv", "sw", "sx", "sz", "ta",  "te", "th",
			   "tig",  "tk", "tlh", "tn", "tr", "ts", "tt", "uk",  "ur", "ve",
			    "vi",  "vo",  "wa", "xh", "zh", "zu" };

bool validateInteger(const char *ptr)
{
	int c = 0;
	
	int len = strlen(ptr);
	
	while (c < len)
	{
		if (!(ptr[c] >= '0' && ptr[c] <= '9'))
		{
			return false;
		}
		
		++c;
	}
	
	return true;
}

bool validateWeight(const char *ptr)
{
	unsigned int c;
	
	if (ptr[0] == '1')
	{
		if (ptr[1] != '\0')
		{
			return false;
		}
		
		return true;
	}
	else if (ptr[0] == '0')
	{
		if (ptr[1] != '.')
		{
			return false;
		}
		
		c = 2;
	}
	else if (ptr[0] != '.')
	{
		return false;
	}
	else // ptr[0] == '.'
	{
		c = 1;
	}
	
	return validateInteger(ptr + c);
}

bool validateLanguageCode(const char *code)
{
	char *codeCopy = (char *)malloc(strlen(code) + 1);
	
	strncpy(codeCopy, code, strlen(code));
	
	codeCopy[strlen(code)] = '\0';
	
	char *ptr = strtok(codeCopy, locality_delim);
	
	if (ptr == NULL)
	{
		return false;
	}

	int len = strlen(ptr);
	
	if (len < 2 || len > 3)
	{
		return false;
	}
	
	unsigned int i = 0;
	
	while (i < sizeof(language_codes) && strcmp(ptr, language_codes[i]) != 0)
	{
		i++;
	}
	
	return i < sizeof(language_codes);
}

int getHTTPFieldValueType(int fieldId)
{
	switch (fieldId)
	{
	case HTTP_FIELD_USER_AGENT:
	{
		return TEXT_VALUE;
	}
	
	case HTTP_FIELD_ACCEPT_LANGUAGE:
	{
		return MULTIPART_WEIGHTED_VALUE;
	}
	
	case HTTP_FIELD_CONTENT_LANGUAGE:
	case HTTP_FIELD_CONNECTION:
	{
		return MULTIPART_VALUE_COMMAS;
	}
	
	case HTTP_FIELD_CONTENT_TYPE:
	{
		return MULTIPART_VALUE_SEMICOLONS;
	}
	
	case HTTP_FIELD_CONTENT_LENGTH:
	{
		return INTEGER_VALUE;
	}
	
	default:
	{
		return UNKNOWN_FIELD;
	}
	}
}

bool validateHTTPResponseCode(int responseCode)
{
	if (responseCode == HTTP_SUCCESS_200_OK ||
		responseCode == HTTP_ERROR_400_BAD_REQUEST ||
		responseCode == HTTP_ERROR_404_NOT_FOUND ||
		responseCode == HTTP_ERROR_500_INTERNAL_SERVER_ERROR ||
		responseCode == HTTP_ERROR_501_NOT_IMPLEMENTED ||
		responseCode == HTTP_ERROR_505_VERSION_NOT_SUPPORTED)
	{
		return true;
	}
	
	return false;
}

bool validateHTTPVersion(const char *httpVersion)
{
	if (strcmp(httpVersion, "1.1") == 0)
	{
		return true;
	}
	
	return false;
}

bool validateFieldValue(int fieldId, const char *fieldValue)
{
	// TODO
	
	return true;
}

char * getHTTPResponseDescription(int responseCode)
{
	char *ret = NULL;
	
	switch (responseCode)
	{
	case HTTP_SUCCESS_200_OK:
	{
		ret = HTTP_SUCCESS_200_OK_MESSAGE;
		break;
	}
	
	case HTTP_ERROR_400_BAD_REQUEST:
	{
		ret = HTTP_ERROR_400_BAD_REQUEST_MESSAGE;
		break;
	}
	
	case HTTP_ERROR_404_NOT_FOUND:
	{
		ret = HTTP_ERROR_404_NOT_FOUND_MESSAGE;
		break;
	}
	
	case HTTP_ERROR_500_INTERNAL_SERVER_ERROR:
	{
		ret = HTTP_ERROR_500_INTERNAL_SERVER_ERROR_MESSAGE;
		break;
	}
	
	case HTTP_ERROR_501_NOT_IMPLEMENTED:
	{
		ret = HTTP_ERROR_501_NOT_IMPLEMENTED_MESSAGE;
		break;
	}
	
	case HTTP_ERROR_505_VERSION_NOT_SUPPORTED:
	{
		ret = HTTP_ERROR_505_VERSION_NOT_SUPPORTED_MESSAGE;
		break;
	}
	}
	
	return ret;
}

int getHTTPFieldId(const char *fieldName)
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

