#include "logger.h"

Logger createlogger(const char *logfile) {
	Logger logger;
	strcpy(logger.logfile, logfile);
	logger.logfile_maxbytes = DEFAULT_LOG_MAXBYTES;
	logger.loglevel = 0;
	logger.logfile_backups = 10;
	logger.capture_maxbytes = 0;
	logger.syslog = 0;
	logger.events_enabled = 0;
	return logger;
}