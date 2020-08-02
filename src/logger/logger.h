#ifndef _LOGGER_H_INCLUDED_
#define _LOGGER_H_INCLUDED_

#define MAX_LINE_LENGTH 1024
#define DEFAULT_LOG_MAXBYTES (50 * 1024 * 1024)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
extern void freelogger(Logger logger);

#endif
