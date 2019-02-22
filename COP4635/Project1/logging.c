#include <stdio.h>
#include <time.h>

#include "logging.h"

void log_header(MSGTYPE type)
{
	time_t timer;
	char buffer[29];
	struct tm* tm_info;
	
	time(&timer);
	tm_info = localtime(&timer);
	
	strftime(buffer, 29, "(%Y-%m-%d %H:%M:%S) ", tm_info);
	
	printf("%s [", buffer);
	
	switch (type)
	{
	case ERROR:
	{
		printf(ANSI_COLOR_RED "ERR ");
		break;
	}
	
	case WARN:
	{
		printf(ANSI_COLOR_YELLOW "WARN");
		break;
	}
	
	case INFO:
	{
		printf(ANSI_COLOR_CYAN "INFO");
		break;
	}
	
	case OK:
	{
		printf(ANSI_COLOR_GREEN " OK ");
		break;
	}
	
	default:
	{
		printf(" ?? ] ");
		return;
	}
	}
	
	printf(ANSI_COLOR_RESET "] ");
}

void log_msg(MSGTYPE type, char *msg)
{
	log_header(type);
	printf("%s\n", msg);
}
