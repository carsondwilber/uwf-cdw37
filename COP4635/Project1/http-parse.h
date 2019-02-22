#include "http.h"

#ifndef HTTP_PARSE_H
#define HTTP_PARSE_H

int parseHTTPFieldMultipartWeightedValue(int fieldId, char *fieldValue, char ***tokens, double **weights);

int parseHTTPFieldMultipart(int fieldId, char *fieldValue, char ***tokens);

int parseHTTPFieldTextValue(int fieldId, char *fieldValue);

int parseHTTPFieldIntegerValue(int fieldId, char *fieldValue, int *value);

int parseHTTPFieldName(char *fieldName);

int parseHTTPRequestHeader(char *data, char **requestType, char **resource, char **httpVersion);

#endif // HTTP_PARSE_H
