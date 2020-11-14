#ifndef _LOGGER_H_INCLUDED_
#define _LOGGER_H_INCLUDED_


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"

typedef enum LOGLEVEL
{
	CRITICAL = 50,
	ERROR = 40,
	WARN = 30,
	INFO = 20,
	DEBUG = 10,
	TRACE = 5,
	BLATHER = 1
} LOGLEVEL;

typedef struct Logger
{
	char logfile[MAX_LINE_LENGTH];
	long long int logfile_maxbytes;
	long long int capture_maxbytes;
	int logfile_backups;
	int loglevel;
	int syslog;
	int events_enabled;
	FILE *logwriter;
} Logger;

extern Logger createlogger(const char *logfile);
extern int openlogger(Logger *logger);
extern int formatlogger(Logger *logger, int loglevel, char *format, ...);
extern int writelogger(Logger *logger, int loglevel, char *msg);
extern void freelogger(Logger logger);

#endif
