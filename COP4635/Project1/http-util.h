#include <stdbool.h>

#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

extern const char multipart_commas_delim[];

extern const char multipart_semicolons_delim[];

extern const char weighted_delim[];

extern const char locality_delim[];

extern const char *language_codes[];

bool validateInteger(const char *ptr);

bool validateWeight(const char *ptr);

bool validateLanguageCode(const char *code);

bool validateHTTPVersion(const char *httpVersion);

bool validateHTTPResponseCode(int responseCode);

bool validateFieldValue(int fieldId, const char *fieldValue);

int getHTTPFieldId(const char *fieldName);

int getHTTPFieldValueType(int fieldId);

char * getHTTPResponseDescription(int responseCode);

#endif // HTTP_UTIL_H
