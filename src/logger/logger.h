#ifndef _LOGGER_H_INCLUDED_
#define _LOGGER_H_INCLUDED_

#define MAX_LINE_LENGTH 1024
#define DEFAULT_LOG_MAXBYTES (50 * 1024 * 1024)

#include <string.h>

typedef struct Logger {
    char logfile[MAX_LINE_LENGTH];
    int logfile_maxbytes;
    int logfile_backups;
    int capture_maxbytes;
	int loglevel;
	int syslog;
	int events_enabled;
} Logger;

extern Logger createlogger(const char *logfile);

#endif
