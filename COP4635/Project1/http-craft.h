#ifndef HTTP_CRAFT_H
#define HTTP_CRAFT_H

int craftHTTPField(const char *fieldName, const char *fieldValue, char *data, int maxBytes);

int craftHTTPResponseHeader(const char *httpVersion, const int responseCode, char *data, int maxBytes);

#endif // HTTP_CRAFT_H
