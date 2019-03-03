/*
 * File: settcptimeout.c
 * Created: 21 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides an interface for setting the timeout time of a TCP connection, as well as case-insensitive string comparisons.
 */

#ifndef UTIL_H
#define UTIL_H

/**
 * Description: 
 * 	This is a utility function that will set a socket's timeout to the specified amount of seconds given
 * by calling the setsockopt(...) function
 *
 * int sockfd:  Takes the id for the socket file descriptor
 * int seconds: The amount of seconds to wait before timing out a tcp socket connection
 */
void settcptimeout(int sockfd, int seconds);

#ifdef _WINDOWS

#define strcicmp strcasecmp

#else // MAC OR LINUX

#include <strings.h>
#define strcicmp strcasecmp

#endif // _WINDOWS

#endif // UTIL_H
