/*
 * File: settcptimeout.c
 * Created: 21 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides implementation for setting the timeout time of a TCP connection.
 */

#include <sys/time.h>
#include <sys/socket.h>

#include "util.h"

void settcptimeout(int sockfd, int seconds)
{
	struct timeval tv;
	tv.tv_sec = seconds;
	tv.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}
