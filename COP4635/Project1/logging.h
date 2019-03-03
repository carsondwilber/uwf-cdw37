/*
 * File: logging.h
 * Created: 17 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * Purpose: Provides an interface for a logging utility, including colorized messages.
 */

#ifndef LOGGING_H
#define LOGGING_H

#define MSGTYPE char

#define WARN 'W'
#define ERROR 'E'
#define INFO 'I'
#define OK 'K'

#define ANSI_COLOR_RED		"\x1b[31m"
#define ANSI_COLOR_GREEN	"\x1b[32m"
#define ANSI_COLOR_YELLOW	"\x1b[33m"
#define ANSI_COLOR_CYAN		"\x1b[36m"
#define ANSI_COLOR_RESET	"\x1b[0m"

/**
 * Description: 
 * 	This is a utility function that will set a socket's timeout to the specified amount of seconds given
 * by calling the setsockopt(...) function
 *
 * MSGTYPE type: Specify the type of message you would like to log via a preprocessor define:
 *			  WARN - W - Warning
 * 			  ERROR- E - Error
 *			  INFO - I - Information
 *			  OK   - K - OK
 *		 Otherwise, it will output prefaced with "?? ]"
 *
 *
 * char* msg:    The message you would like to output along with the proper ANSI coloring
 */
void log_msg(MSGTYPE type, char *msg);

#endif // LOGGING_H
