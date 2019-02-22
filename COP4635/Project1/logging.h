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

void log_msg(MSGTYPE type, char *msg);

#endif // LOGGING_H
