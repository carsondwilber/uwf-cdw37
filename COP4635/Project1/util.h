#ifndef UTIL_H
#define UTIL_H

void settcptimeout(int sockfd, int seconds);

#ifdef _WINDOWS

#define strcicmp strcasecmp

#else // MAC OR LINUX

#include <strings.h>
#define strcicmp strcasecmp

#endif // _WINDOWS

#endif // UTIL_H
